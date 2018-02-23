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
  ): gpu::GPU(
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
    initialized = false;
    splitCount = 8;

    // Set up constants.
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    auto grid = memories.grid.at(0);
    grid->edgeLength = gridEdgeLength;
    grid->xCount = xGridCount;
    grid->yCount = yGridCount;
    grid->zCount = zGridCount;
    grid->origin = {
      -(float)((xGridCount * gridEdgeLength) / 2),
      -(float)((yGridCount * gridEdgeLength) / 2),
      -(float)((zGridCount * gridEdgeLength) / 2)
    };
    for (int i = 0; i < 3; i++) {
      grid->normals[i] = {0.0f, 0.0f, 0.0f};
      grid->normals[i].s[i] = 1.0f;
    }
    for (int i = 3; i < 6; i++) {
      grid->normals[i] = {0.0f, 0.0f, 0.0f};
      grid->normals[i].s[i - 3] = -1.0f;
    }

    auto fluidSettings = memories.fluidSettings.at(0);

    fluidSettings->stiffness = 16.0f;
    fluidSettings->density = 0.1f;
    fluidSettings->viscosity = 8.0f;
    fluidSettings->particleMass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * fluidSettings->density;
    fluidSettings->effectiveRadius = 2.0f;
    fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(fluidSettings->effectiveRadius, 9));
    fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
    fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
  }

  void Simulator::setUpComputingSize() {
    softBodyParticleCount = (unsigned int)softBodyParticles.size();
    fluidParticleCount = (unsigned int)fluidParticles.size();
    actorCount = softBodyParticleCount + fluidParticleCount;
    springCount = (unsigned int)springs.size();
    actorCountForBitonicSort = utils::math::powerOf2(actorCount);
  }

  void Simulator::initGPUMemory(float deltaTime) {
    float splitDeltaTime = deltaTime / splitCount;
    kernels.inputConstants(
      1,
      memories.constants,
      memories.grid,
      memories.fluidSettings,
      gravity,
      deltaTime,
      splitDeltaTime,
      sphericalShellRadius,
      (unsigned short)softBodyParticleCount
    );

    memories.hostFluidStates.write();
    kernels.inputFluid(
      (unsigned short)fluidParticleCount,
      memories.hostFluidStates,
      memories.fluidStates
    );
  }

  void Simulator::setUpSpring(unsigned int springIndex, unsigned char nodeIndex) {
    unsigned short actorIndex = springs[springIndex]->nodes[nodeIndex].particle->index;
    memories.springs.at(springIndex)->actorIndices[nodeIndex] = actorIndex;
    memories.springs.at(springIndex)->nodePositionsModelSpace[nodeIndex] = toCl(springs[springIndex]->nodes[nodeIndex].position);
    auto actor = memories.actors.at(actorIndex);
    actor->springIndices[actor->springCount] = springIndex;
    actor->springNodeIndices[actor->springCount] = nodeIndex;
    actor->springCount++;
  }

  void Simulator::input() {
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      auto actor = memories.actors.at(i);
      auto hostPhysicalQuantity = memories.hostPhysicalQuantities.at(i);
      actor->radius = softBodyParticle->radius;
      actor->mass = softBodyParticle->mass;
      actor->elasticity = softBodyParticle->elasticity;
      actor->staticFrictionCoefficient = softBodyParticle->staticFrictionCoefficient;
      actor->dynamicFrictionCoefficient = softBodyParticle->dynamicFrictionCoefficient;
      actor->radiusForAlterEgo = softBodyParticle->radiusForAlterEgo;
      actor->type = 0;

      hostPhysicalQuantity->linearMomentum = toCl(softBodyParticle->linearMomentum);
      hostPhysicalQuantity->angularMomentum = toCl(softBodyParticle->angularMomentum);
      hostPhysicalQuantity->position = toCl(softBodyParticle->position);
      hostPhysicalQuantity->rotation = toCl(softBodyParticle->rotation);
      softBodyParticle->index = (unsigned short)i;
      actor->springCount = 0;
    }
    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + softBodyParticleCount;
      FluidParticle* fluidParticle = fluidParticles[i];
      auto actor = memories.actors.at(globalIndex);
      auto hostPhysicalQuantity = memories.hostPhysicalQuantities.at(globalIndex);
      actor->radius = memories.fluidSettings.at(0)->effectiveRadius / 2.0f;
      actor->mass = memories.fluidSettings.at(0)->particleMass;
      actor->elasticity = 0.0f;
      actor->staticFrictionCoefficient = 0.0f;
      actor->dynamicFrictionCoefficient = 0.0f;
      actor->radiusForAlterEgo = 1.0f;
      actor->alterEgoIndex = -1;
      actor->type = 3;

      cl_float3 clFloat3Zero = {0.0f, 0.0f, 0.0f};
      hostPhysicalQuantity->linearMomentum = clFloat3Zero;
      hostPhysicalQuantity->angularMomentum = clFloat3Zero;
      hostPhysicalQuantity->position = toCl(fluidParticle->position);
      hostPhysicalQuantity->rotation = {0.0f, 0.0f, 0.0f, 1.0f};
      actor->springCount = 0;

      auto fluidState = memories.hostFluidStates.at(i);
      fluidState->density = 0.0f;
      fluidState->force = clFloat3Zero;
      fluidState->pressure = 0.0f;
      fluidState->velocity = clFloat3Zero;
    }
    for (unsigned int i = 0; i < springCount; i++) {
      memories.springs.at(i)->k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      if (softBodyParticle->alterEgo == nullptr) {
        memories.actors.at(i)->alterEgoIndex = -1;
      } else {
        memories.actors.at(i)->alterEgoIndex = softBodyParticle->alterEgo->index;
      }
    }
  }

  void Simulator::output() {
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      auto physicalQuantity = memories.physicalQuantities.at(i);
      softBodyParticle->linearMomentum = toGlm(physicalQuantity->linearMomentum);
      softBodyParticle->angularMomentum = toGlm(physicalQuantity->angularMomentum);
      softBodyParticle->position = toGlm(physicalQuantity->position);
      softBodyParticle->rotation = toGlmQuat(physicalQuantity->rotation);
    }

    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + softBodyParticleCount;
      FluidParticle* particle = fluidParticles[i];
      particle->position = toGlm(memories.physicalQuantities.at(globalIndex)->position);
    }
  }

  void Simulator::setUpMemories() {
    memories.actors.setCount(actorCount);
    memories.actorStates.setCount(actorCount);
    memories.hostPhysicalQuantities.setCount(actorCount);
    memories.physicalQuantities.setCount(actorCount);
    memories.gridAndActorRelations.setCount(actorCountForBitonicSort);
    memories.springs.setCount(springCount);
    memories.springVars.setCount(springCount);
    memories.fluidStates.setCount(fluidParticleCount);
    memories.hostFluidStates.setCount(fluidParticleCount);
    memories.actors.write();
    memories.hostPhysicalQuantities.write();
    memories.springs.write();
  }

  void Simulator::computeBroadPhase() {
    auto maxParticleCountShort = (unsigned short)maxActorCount;
    // Initialize grid and particle relations
    kernels.initGridAndActorRelations(
      actorCountForBitonicSort,
      memories.gridAndActorRelations,
      allGridCount,
      maxParticleCountShort
    );

    // Set grid index to rigid body state, and register grid and particle relations.
    kernels.fillGridIndex(
      actorCount,
      memories.grid,
      memories.actors,
      memories.actorStates,
      memories.hostPhysicalQuantities,
      memories.physicalQuantities,
      memories.gridAndActorRelations
    );

    // Sort grid and particle relations (Bitonic sort)
    auto stageCount = (int)log2(actorCountForBitonicSort);
    int passCount = 0;
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
        kernels.bitonic(
          actorCountForBitonicSort,
          memories.gridAndActorRelations,
          distance,
          stageDistance
        );
      }
      passCount++;
    }
    passCount = stageCount;
    for (int i = 0; i < passCount; i++) {
      auto distance = (unsigned int)(1 << (stageCount - (i + 1)));
      kernels.merge(
        actorCountForBitonicSort,
        memories.gridAndActorRelations,
        distance
      );
    }

    // Setup grid and particle relation ranges
    memories.gridStartIndices.zeroFill();
    memories.gridEndIndices.zeroFill();
    kernels.setGridRelationIndexRange(
      actorCount > 1 ? actorCount - 1 : 1,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      actorCount
    );
  }

  void Simulator::computeNarrowPhase() {
    kernels.collectIntersections(
      actorCount,
      memories.actorStates,
      memories.springs,
      memories.physicalQuantities,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      memories.constants
    );
  }

  void Simulator::resolveConstraints(float deltaTime) {
    kernels.updateByPenaltyImpulse(
      softBodyParticleCount,
      memories.actors,
      memories.actorStates,
      deltaTime
    );

    for (int i = 0; i < 16; i++) {
      kernels.collectCollisions(
        softBodyParticleCount,
        memories.actors,
        memories.actorStates
      );

      kernels.updateByConstraintImpulse(
        softBodyParticleCount,
        memories.actors,
        memories.actorStates
      );
    }
    kernels.updateByFrictionalImpulse(
      softBodyParticleCount,
      memories.actors,
      memories.actorStates
    );
  }

  void Simulator::motion(float deltaTime) {
    float splitDeltaTime = deltaTime / splitCount;
    for (int i = 0; i < splitCount; i++) {
      if (springCount > 0) {
        kernels.calcSpringImpulses(
          springCount,
          memories.actorStates,
          memories.springs,
          memories.springVars,
          memories.physicalQuantities,
          splitDeltaTime
        );
      }
      kernels.updateBySpringImpulse(
        softBodyParticleCount,
        memories.actors,
        memories.actorStates,
        memories.physicalQuantities,
        memories.springVars,
        splitDeltaTime
      );
    }
    kernels.postProcessing(
      softBodyParticleCount,
      memories.grid,
      memories.actors,
      memories.actorStates,
      memories.physicalQuantities,
      deltaTime
    );
  }

  void Simulator::updateFluid() {
    kernels.updateDensityAndPressure(
      fluidParticleCount,
      memories.actorStates,
      memories.fluidStates,
      memories.constants
    );

    kernels.updateFluidForce(
      fluidParticleCount,
      memories.actorStates,
      memories.fluidStates,
      memories.constants
    );

    kernels.moveFluid(
      fluidParticleCount,
      memories.fluidStates,
      memories.physicalQuantities,
      memories.constants
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
    updateFluid();
    memories.physicalQuantities.read();
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
