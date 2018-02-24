#include "Simulator.h"


namespace alcube::physics {
  using namespace utils::opencl::kernelargs;

  Simulator::Simulator(
    utils::opencl::ResourcesProvider *resourcesProvider,
    unsigned int maxActorCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount,
    float deltaTime
  ): gpu::GPU(
    resourcesProvider,
    maxActorCount,
    utils::math::powerOf2(maxActorCount),
    maxActorCount * 16,
    xGridCount * yGridCount * zGridCount
  ) {
    this->deltaTime = deltaTime;
    this->maxActorCount = maxActorCount;
    this->allGridCount = xGridCount * yGridCount * zGridCount;
    softBodyParticles = {};
    springs = {};
    fluidParticles = {};
    initialized = false;
    motionIterationCount = 8;
    constraintResolvingIterationCount = 16;

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

  void Simulator::setUpSpring(unsigned int springIndex, unsigned char nodeIndex) {
    unsigned short actorIndex = springs[springIndex]->nodes[nodeIndex].particle->index;
    memories.springs.at(springIndex)->actorIndices[nodeIndex] = actorIndex;
    memories.springs.at(springIndex)->nodePositionsModelSpace[nodeIndex] = toCl(springs[springIndex]->nodes[nodeIndex].position);

    unsigned short softBodyIndex = memories.actors.at(actorIndex)->subPhysicalQuantityIndex;
    auto hostSoftBodyState = memories.hostSoftBodyStates.at(softBodyIndex);
    hostSoftBodyState->springIndices[hostSoftBodyState->springCount] = springIndex;
    hostSoftBodyState->springNodeIndices[hostSoftBodyState->springCount] = nodeIndex;
    hostSoftBodyState->springCount++;
  }

  void Simulator::input() {
    // Input soft body particles
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      auto actor = memories.actors.at(i);
      auto hostPhysicalQuantity = memories.hostPhysicalQuantities.at(i);
      actor->radius = softBodyParticle->radius;
      actor->mass = softBodyParticle->mass;
      actor->type = 0;
      actor->subPhysicalQuantityIndex = (unsigned short)i;
      hostPhysicalQuantity->linearMomentum = toCl(softBodyParticle->linearMomentum);
      hostPhysicalQuantity->angularMomentum = toCl(softBodyParticle->angularMomentum);
      hostPhysicalQuantity->position = toCl(softBodyParticle->position);
      hostPhysicalQuantity->rotation = toCl(softBodyParticle->rotation);

      auto hostSoftBodyState = memories.hostSoftBodyStates.at(i);
      hostSoftBodyState->elasticity = softBodyParticle->elasticity;
      hostSoftBodyState->staticFrictionCoefficient = softBodyParticle->staticFrictionCoefficient;
      hostSoftBodyState->dynamicFrictionCoefficient = softBodyParticle->dynamicFrictionCoefficient;
      hostSoftBodyState->springCount = 0;
      hostSoftBodyState->actorIndex = (unsigned short)i;
      softBodyParticle->index = (unsigned short)i;
    }

    // Input springs
    for (unsigned int i = 0; i < springCount; i++) {
      memories.springs.at(i)->k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }

    // Input fluid particles
    for (unsigned short i = 0; i < fluidParticleCount; i++) {
      auto actorIndex = (unsigned short)(i + softBodyParticleCount);
      FluidParticle* fluidParticle = fluidParticles[i];
      auto actor = memories.actors.at(actorIndex);
      auto hostPhysicalQuantity = memories.hostPhysicalQuantities.at(actorIndex);
      actor->radius = memories.fluidSettings.at(0)->effectiveRadius / 2.0f;
      actor->mass = memories.fluidSettings.at(0)->particleMass;
      actor->type = 3;
      actor->subPhysicalQuantityIndex = i;

      cl_float3 clFloat3Zero = {0.0f, 0.0f, 0.0f};
      hostPhysicalQuantity->linearMomentum = clFloat3Zero;
      hostPhysicalQuantity->angularMomentum = clFloat3Zero;
      hostPhysicalQuantity->position = toCl(fluidParticle->position);
      hostPhysicalQuantity->rotation = {0.0f, 0.0f, 0.0f, 1.0f};

      auto fluidState = memories.hostFluidStates.at(i);
      fluidState->density = 0.0f;
      fluidState->force = clFloat3Zero;
      fluidState->pressure = 0.0f;
      fluidState->velocity = clFloat3Zero;
      fluidState->actorIndex = actorIndex;
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
    memories.hostPhysicalQuantities.setCount(actorCount);
    memories.hostSoftBodyStates.setCount(softBodyParticleCount);
    memories.springs.setCount(springCount);
    memories.hostFluidStates.setCount(fluidParticleCount);

    memories.actorStates.setCount(actorCount);
    memories.physicalQuantities.setCount(actorCount);
    memories.gridAndActorRelations.setCount(actorCountForBitonicSort);
    memories.springStates.setCount(springCount);
    memories.fluidStates.setCount(fluidParticleCount);

    memories.actors.write();
    memories.hostPhysicalQuantities.write();
    memories.hostSoftBodyStates.write();
    memories.springs.write();
    memories.hostFluidStates.write();

    float splitDeltaTime = deltaTime / motionIterationCount;
    kernels.inputConstants(
      1,
      memories.constants,
      memories.grid,
      memories.fluidSettings,
      gravity,
      deltaTime,
      splitDeltaTime,
      sphericalShellRadius
    );

    kernels.inputActors(
      actorCount,
      memories.actors,
      memories.actorStates,
      memories.hostPhysicalQuantities,
      memories.physicalQuantities
    );

    kernels.inputSoftBodyStates(
      softBodyParticleCount,
      memories.hostSoftBodyStates,
      memories.softBodyStates
    );

    kernels.inputFluid(
      (unsigned short)fluidParticleCount,
      memories.hostFluidStates,
      memories.fluidStates
    );
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
      memories.constants,
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
      memories.physicalQuantities,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      memories.constants
    );
  }

  void Simulator::resolveConstraints() {
    kernels.updateByPenaltyImpulse(
      softBodyParticleCount,
      memories.actors,
      memories.actorStates,
      deltaTime
    );

    for (int i = 0; i < constraintResolvingIterationCount; i++) {
      kernels.collectCollisions(
        softBodyParticleCount,
        memories.actors,
        memories.actorStates
      );

      kernels.updateByConstraintImpulse(
        softBodyParticleCount,
        memories.actors,
        memories.actorStates,
        memories.softBodyStates
      );
    }
    kernels.updateByFrictionalImpulse(
      softBodyParticleCount,
      memories.actors,
      memories.actorStates
    );
  }

  void Simulator::motion() {
    float splitDeltaTime = deltaTime / motionIterationCount;
    for (int i = 0; i < motionIterationCount; i++) {
      if (springCount > 0) {
        kernels.calcSpringImpulses(
          springCount,
          memories.springs,
          memories.springStates,
          memories.physicalQuantities,
          splitDeltaTime
        );
      }
      kernels.updateBySpringImpulse(
        softBodyParticleCount,
        memories.softBodyStates,
        memories.actorStates,
        memories.physicalQuantities,
        memories.springStates,
        splitDeltaTime
      );
    }
    kernels.postProcessing(
      softBodyParticleCount,
      memories.constants,
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

  void Simulator::update() {
    if (!initialized) {
      setUpComputingSize();
      input();
      setUpMemories();
      initialized = true;
    }

    computeBroadPhase();
    computeNarrowPhase();
    resolveConstraints();
    motion();
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
