#include "Simulator.h"

namespace alcube::physics {
  using namespace utils::opencl::kernelargs;
  Simulator::Simulator(
    utils::opencl::ResourcesProvider *resourcesProvider,
    unsigned int maxActorCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount
  ) : utils::opencl::Simulator(resourcesProvider), gpu(
    resourcesProvider,
    maxActorCount,
    utils::math::powerOf2(maxActorCount),
    maxActorCount * 16,
    xGridCount * yGridCount * zGridCount
  ) {
    this->maxActorCount = maxActorCount;
    this->allGridCount = xGridCount * yGridCount * zGridCount;
    softBodyParticles = {};
    springs = {};
    fluidParticles = {};
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    auto grid = gpu.memories.grid.at(0);
    grid->edgeLength = gridEdgeLength;
    grid->xCount = xGridCount;
    grid->yCount = yGridCount;
    grid->zCount = zGridCount;
    grid->origin.s[0] = -(float)((xGridCount * gridEdgeLength) / 2);
    grid->origin.s[1] = -(float)((yGridCount * gridEdgeLength) / 2);
    grid->origin.s[2] = -(float)((zGridCount * gridEdgeLength) / 2);
    for (int i = 0; i < 3; i++) {
      grid->normals[i].s[0] = 0.0f;
      grid->normals[i].s[0] = 0.0f;
      grid->normals[i].s[0] = 0.0f;
      grid->normals[i].s[i] = 1.0f;
    }
    for (int i = 3; i < 6; i++) {
      grid->normals[i].s[0] = 0.0f;
      grid->normals[i].s[0] = 0.0f;
      grid->normals[i].s[0] = 0.0f;
      grid->normals[i].s[i - 3] = -1.0f;
    }

    auto fluidSettings = gpu.memories.fluidSettings.at(0);

    fluidSettings->stiffness = 16.0f;
    fluidSettings->density = 0.1f;
    fluidSettings->viscosity = 8.0f;
    fluidSettings->particleMass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * fluidSettings->density;
    fluidSettings->effectiveRadius = 2.0f;
    fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(fluidSettings->effectiveRadius, 9));
    fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
    fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));

    initialized = false;
  }

  void Simulator::setUpComputingSize() {
    softBodyParticleCount = (unsigned int)softBodyParticles.size();
    fluidParticleCount = (unsigned int)fluidParticles.size();
    actorCount = softBodyParticleCount + fluidParticleCount;
    springCount = (unsigned int)springs.size();
    actorCountForBitonicSort = utils::math::powerOf2(actorCount);
  }

  void Simulator::setUpSpring(unsigned int springIndex, unsigned char nodeIndex) {
    unsigned short actorIndex = springs[springIndex]->nodes[nodeIndex].particle->index;
    gpu.memories.springs.at(springIndex)->actorIndices[nodeIndex] = actorIndex;
    assignClFloat3(gpu.memories.springs.at(springIndex)->nodePositionsModelSpace[nodeIndex], springs[springIndex]->nodes[nodeIndex].position);
    auto actor = gpu.memories.actors.at(actorIndex);
    actor->springIndices[actor->springCount] = springIndex;
    actor->springNodeIndices[actor->springCount] = nodeIndex;
    actor->springCount++;
  }

  void Simulator::initGPUMemory(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    auto rigidBodyParticleCountShort = (unsigned short)softBodyParticleCount;
    auto fluidParticleCountShort = (unsigned short)fluidParticleCount;
    gpu.kernels.inputConstants(
      1,
      gpu.memories.constants,
      gpu.memories.grid,
      gpu.memories.fluidSettings,
      gravity,
      deltaTime,
      splitDeltaTime,
      sphericalShellRadius,
      rigidBodyParticleCountShort,
      fluidParticleCountShort
    );

    gpu.resourcesProvider->queue->write(gpu.memories.inputFluidStates.memory);
    gpu.kernels.inputFluid(fluidParticleCountShort, gpu.memories.inputFluidStates, gpu.memories.fluidStates);
  }

  void Simulator::input() {

    glm::vec3 vec3Zero = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      auto actor = gpu.memories.actors.at(i);
      auto currentState = gpu.memories.currentStates.at(i);
      actor->radius = softBodyParticle->radius;
      actor->mass = softBodyParticle->mass;
      actor->elasticity = softBodyParticle->elasticity;
      actor->staticFrictionCoefficient = softBodyParticle->staticFrictionCoefficient;
      actor->dynamicFrictionCoefficient = softBodyParticle->dynamicFrictionCoefficient;
      actor->radiusForAlterEgo = softBodyParticle->radiusForAlterEgo;
      actor->type = 0;
      assignClFloat3(currentState->linearMomentum, softBodyParticle->linearMomentum);
      assignClFloat3(currentState->angularMomentum, softBodyParticle->angularMomentum);
      assignClFloat3(currentState->position, softBodyParticle->position);
      assignClFloat4(currentState->rotation, softBodyParticle->rotation);
      softBodyParticle->index = (unsigned short)i;
      actor->springCount = 0;
    }
    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + softBodyParticleCount;
      FluidParticle* fluidParticle = fluidParticles[i];
      auto actor = gpu.memories.actors.at(globalIndex);
      auto currentStates = gpu.memories.currentStates.at(globalIndex);
      actor->radius = gpu.memories.fluidSettings.at(0)->effectiveRadius / 2.0f;
      actor->mass = gpu.memories.fluidSettings.at(0)->particleMass;
      actor->elasticity = 0.0f;
      actor->staticFrictionCoefficient = 0.0f;
      actor->dynamicFrictionCoefficient = 0.0f;
      actor->radiusForAlterEgo = 1.0f;
      actor->alterEgoIndex = -1;
      actor->type = 3;
      glm::quat quatIdent = glm::quat();
      assignClFloat3(currentStates->linearMomentum, vec3Zero);
      assignClFloat3(currentStates->angularMomentum, vec3Zero);
      assignClFloat3(currentStates->position, fluidParticle->position);
      assignClFloat4(currentStates->rotation, quatIdent);
      actor->springCount = 0;

      auto fluidState = gpu.memories.inputFluidStates.at(i);
      fluidState->density = 0.0f;
      assignClFloat3(fluidState->force, vec3Zero);
      fluidState->pressure = 0.0f;
      assignClFloat3(fluidState->velocity, vec3Zero);
    }
    for (unsigned int i = 0; i < springCount; i++) {
      gpu.memories.springs.at(i)->k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      if (softBodyParticle->alterEgo == nullptr) {
        gpu.memories.actors.at(i)->alterEgoIndex = -1;
      } else {
        gpu.memories.actors.at(i)->alterEgoIndex = softBodyParticle->alterEgo->index;
      }
    }
  }

  void Simulator::output() {
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      auto nextState = gpu.memories.nextStates.at(i);
      assignGlmVec3(softBodyParticle->linearMomentum, nextState->linearMomentum);
      assignGlmVec3(softBodyParticle->angularMomentum, nextState->angularMomentum);
      assignGlmVec3(softBodyParticle->position, nextState->position);
      assignGlmQuat(softBodyParticle->rotation, nextState->rotation);
    }

    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + softBodyParticleCount;
      FluidParticle* particle = fluidParticles[i];
      assignGlmVec3(particle->position, gpu.memories.nextStates.at(globalIndex)->position);
    }
  }

  void Simulator::setUpMemories() {
    gpu.memories.actors.memory->count = actorCount;
    gpu.memories.actorStates.memory->count = actorCount;
    gpu.memories.currentStates.memory->count = actorCount;
    gpu.memories.nextStates.memory->count = actorCount;
    gpu.memories.gridAndActorRelations.memory->count = actorCountForBitonicSort;
    gpu.memories.springs.memory->count = springCount;
    gpu.memories.springVars.memory->count = springCount;
    gpu.memories.fluidStates.memory->count = fluidParticleCount;
    gpu.memories.inputFluidStates.memory->count = fluidParticleCount;
    gpu.resourcesProvider->queue->write(gpu.memories.actors.memory);
    gpu.resourcesProvider->queue->write(gpu.memories.currentStates.memory);
    gpu.resourcesProvider->queue->write(gpu.memories.springs.memory);
  }

  void Simulator::computeBroadPhase() {
    auto maxParticleCountShort = (unsigned short)maxActorCount;
    // Initialize grid and particle relations
    gpu.kernels.initGridAndActorRelations(
      actorCountForBitonicSort,
      gpu.memories.gridAndActorRelations,
      allGridCount,
      maxParticleCountShort
    );

    // Set grid index to rigid body state, and register grid and particle relations.
    gpu.kernels.fillGridIndex(
      actorCount,
      gpu.memories.grid,
      gpu.memories.actors,
      gpu.memories.actorStates,
      gpu.memories.currentStates,
      gpu.memories.nextStates,
      gpu.memories.gridAndActorRelations
    );

    // Sort grid and particle relations (Bitonic sort)
    auto stageCount = (int)log2(actorCountForBitonicSort);
    int passCount = 0;
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
        gpu.kernels.bitonic(
          actorCountForBitonicSort,
          gpu.memories.gridAndActorRelations,
          distance,
          stageDistance
        );
      }
      passCount++;
    }
    passCount = stageCount;
    for (int i = 0; i < passCount; i++) {
      auto distance = (unsigned int)(1 << (stageCount - (i + 1)));
      gpu.kernels.merge(
        actorCountForBitonicSort,
        gpu.memories.gridAndActorRelations,
        distance
      );
    }

    // Setup grid and particle relation ranges
    gpu.resourcesProvider->queue->pushZeroFill(gpu.memories.gridStartIndices.memory);
    gpu.resourcesProvider->queue->pushZeroFill(gpu.memories.gridEndIndices.memory);
    gpu.kernels.setGridRelationIndexRange(
      actorCount > 1 ? actorCount - 1 : 1,
      gpu.memories.gridAndActorRelations,
      gpu.memories.gridStartIndices,
      gpu.memories.gridEndIndices,
      actorCount
    );
  }

  void Simulator::computeNarrowPhase() {
    gpu.kernels.collectIntersections(
      actorCount,
      gpu.memories.actorStates,
      gpu.memories.springs,
      gpu.memories.nextStates,
      gpu.memories.gridAndActorRelations,
      gpu.memories.gridStartIndices,
      gpu.memories.gridEndIndices,
      gpu.memories.constants
    );
  }

  void Simulator::resolveConstraints(float deltaTime) {
    gpu.kernels.updateByPenaltyImpulse(
      softBodyParticleCount,
      gpu.memories.actors,
      gpu.memories.actorStates,
      deltaTime
    );

    for (int i = 0; i < 16; i++) {
      gpu.kernels.collectCollisions(
        softBodyParticleCount,
        gpu.memories.actors,
        gpu.memories.actorStates
      );

      gpu.kernels.updateByConstraintImpulse(
        softBodyParticleCount,
        gpu.memories.actors,
        gpu.memories.actorStates
      );
    }
    gpu.kernels.updateByFrictionalImpulse(
      softBodyParticleCount,
      gpu.memories.actors,
      gpu.memories.actorStates
    );
  }

  void Simulator::motion(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    for (int i = 0; i < splitCount; i++) {
      if (springCount > 0) {
        gpu.kernels.calcSpringImpulses(
          springCount,
          gpu.memories.actorStates,
          gpu.memories.springs,
          gpu.memories.springVars,
          gpu.memories.nextStates,
          splitDeltaTime
        );
      }
      gpu.kernels.updateBySpringImpulse(
        softBodyParticleCount,
        gpu.memories.actors,
        gpu.memories.actorStates,
        gpu.memories.nextStates,
        gpu.memories.springVars,
        splitDeltaTime
      );
    }
    gpu.kernels.postProcessing(
      softBodyParticleCount,
      gpu.memories.grid,
      gpu.memories.actors,
      gpu.memories.actorStates,
      gpu.memories.nextStates,
      deltaTime
    );
  }

  void Simulator::updateFluid(float deltaTime) {
    gpu.kernels.updateDensityAndPressure(
      fluidParticleCount,
      gpu.memories.actorStates,
      gpu.memories.fluidStates,
      gpu.memories.constants
    );

    gpu.kernels.updateFluidForce(
      fluidParticleCount,
      gpu.memories.actorStates,
      gpu.memories.fluidStates,
      gpu.memories.constants
    );

    gpu.kernels.moveFluid(
      fluidParticleCount,
      gpu.memories.fluidStates,
      gpu.memories.nextStates,
      gpu.memories.constants
    );
  }

  void Simulator::update(float deltaTime) {
    setUpComputingSize();
    input();
    setUpMemories();
    if (!initialized) {
      initGPUMemory(deltaTime);
      initialized = true;
    }

    computeBroadPhase();
    computeNarrowPhase();
    resolveConstraints(deltaTime);
    motion(deltaTime);
    updateFluid(deltaTime);
    read(gpu.memories.nextStates.memory, gpu.memories.nextStates.dto);
    output();
  }

  void Simulator::add(SoftBodyParticle *softBodyParticle) {
    softBodyParticles.push_back(softBodyParticle);
  }

  SoftBodyParticle* Simulator::getSoftBodyParticle(unsigned long i) {
    return softBodyParticles[i];
  }

  void Simulator::add(Spring *spring) {
    springs.push_back(spring);
  }

  void Simulator::add(FluidParticle *fluidParticle) {
    fluidParticles.push_back(fluidParticle);
  }
}
