#include "Simulator.h"


namespace alcube::physics {
  using namespace utils::opencl::kernelargs;

  Simulator::Simulator(
    unsigned int maxActorCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount,
    float deltaTime,
    gpu::GPUAccessor* gpu
  ) {
    kernels = gpu->kernels;
    memories = gpu->memories;
    this->deltaTime = deltaTime;
    this->maxActorCount = maxActorCount;
    this->allGridCount = xGridCount * yGridCount * zGridCount;
    softBodyActors = {};
    springs = {};
    fluidActors = {};
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

    fluidSettings->stiffness = 64.0f;
    fluidSettings->density = 0.1f;
    fluidSettings->viscosity = 8.0f;
    fluidSettings->particleMass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * fluidSettings->density;
    fluidSettings->effectiveRadius = 2.0f;
    fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(fluidSettings->effectiveRadius, 9));
    fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
    fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
  }

  void Simulator::setUpComputingSize() {
    softBodyActorCount = (unsigned int)softBodyActors.size();
    fluidActorCount = (unsigned int)fluidActors.size();
    actorCount = softBodyActorCount + fluidActorCount;
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

  void Simulator::writeHostMemories() {
    // Input soft body actors
    for (int i = 0; i < softBodyActorCount; i++) {
      SoftBodyActor* softBodyActor = softBodyActors[i];
      auto actor = memories.actors.at(i);
      auto hostPhysicalQuantity = memories.hostPhysicalQuantities.at(i);
      hostPhysicalQuantity->radius = softBodyActor->radius;
      hostPhysicalQuantity->mass = softBodyActor->mass;
      actor->type = 0;
      actor->subPhysicalQuantityIndex = (unsigned short)i;
      hostPhysicalQuantity->linearMomentum = toCl(softBodyActor->linearMomentum);
      hostPhysicalQuantity->angularMomentum = toCl(softBodyActor->angularMomentum);
      hostPhysicalQuantity->position = toCl(softBodyActor->position);
      hostPhysicalQuantity->rotation = toCl(softBodyActor->rotation);

      auto hostSoftBodyState = memories.hostSoftBodyStates.at(i);
      hostSoftBodyState->elasticity = softBodyActor->elasticity;
      hostSoftBodyState->staticFrictionCoefficient = softBodyActor->staticFrictionCoefficient;
      hostSoftBodyState->dynamicFrictionCoefficient = softBodyActor->dynamicFrictionCoefficient;
      hostSoftBodyState->springCount = 0;
      hostSoftBodyState->actorIndex = (unsigned short)i;
      softBodyActor->index = (unsigned short)i;
    }

    // Input springs
    for (unsigned int i = 0; i < springCount; i++) {
      memories.springs.at(i)->k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }

    // Input fluid actors
    for (unsigned short i = 0; i < fluidActorCount; i++) {
      auto actorIndex = (unsigned short)(i + softBodyActorCount);
      FluidActor* fluidActor = fluidActors[i];
      auto actor = memories.actors.at(actorIndex);
      auto hostPhysicalQuantity = memories.hostPhysicalQuantities.at(actorIndex);
      hostPhysicalQuantity->radius = memories.fluidSettings.at(0)->effectiveRadius / 2.0f;
      hostPhysicalQuantity->mass = memories.fluidSettings.at(0)->particleMass;
      actor->type = 3;
      actor->subPhysicalQuantityIndex = i;

      cl_float3 clFloat3Zero = {0.0f, 0.0f, 0.0f};
      hostPhysicalQuantity->linearMomentum = clFloat3Zero;
      hostPhysicalQuantity->angularMomentum = clFloat3Zero;
      hostPhysicalQuantity->position = toCl(fluidActor->position);
      hostPhysicalQuantity->rotation = {0.0f, 0.0f, 0.0f, 1.0f};

      auto fluidState = memories.hostFluidStates.at(i);
      fluidState->density = 0.0f;
      fluidState->force = clFloat3Zero;
      fluidState->pressure = 0.0f;
      fluidState->actorIndex = actorIndex;
    }
  }

  void Simulator::output() {
    memories.physicalQuantities.read();
    for (int i = 0; i < softBodyActorCount; i++) {
      SoftBodyActor* softBodyActor = softBodyActors[i];
      auto physicalQuantity = memories.physicalQuantities.at(i);
      softBodyActor->linearMomentum = toGlm(physicalQuantity->linearMomentum);
      softBodyActor->angularMomentum = toGlm(physicalQuantity->angularMomentum);
      softBodyActor->position = toGlm(physicalQuantity->position);
      softBodyActor->rotation = toGlmQuat(physicalQuantity->rotation);
    }

    for (int i = 0; i < fluidActorCount; i++) {
      int globalIndex = i + softBodyActorCount;
      FluidActor* fluidActor = fluidActors[i];
      fluidActor->position = toGlm(memories.physicalQuantities.at(globalIndex)->position);
    }
  }

  void Simulator::setUpMemories() {
    memories.actors.setCount(actorCount);
    memories.hostPhysicalQuantities.setCount(actorCount);
    memories.hostSoftBodyStates.setCount(softBodyActorCount);
    memories.springs.setCount(springCount);
    memories.hostFluidStates.setCount(fluidActorCount);

    memories.actorStates.setCount(actorCount);
    memories.physicalQuantities.setCount(actorCount);
    memories.gridAndActorRelations.setCount(actorCountForBitonicSort);
    memories.springStates.setCount(springCount);
    memories.fluidStates.setCount(fluidActorCount);

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
      softBodyActorCount,
      memories.hostSoftBodyStates,
      memories.softBodyStates
    );

    kernels.inputSprings(
      springCount,
      memories.springs,
      memories.springStates
    );

    kernels.inputFluid(
      (unsigned short)fluidActorCount,
      memories.hostFluidStates,
      memories.fluidStates
    );
  }

  void Simulator::computeBroadPhase() {
    auto maxActorCountShort = (unsigned short)maxActorCount;
    // Initialize grid and actor relations
    kernels.initGridAndActorRelations(
      actorCountForBitonicSort,
      memories.gridAndActorRelations,
      allGridCount,
      maxActorCountShort
    );

    // Set grid index to rigid body state, and register grid and actor relations.
    kernels.fillGridIndex(
      actorCount,
      memories.constants,
      memories.physicalQuantities,
      memories.gridAndActorRelations
    );

    // Sort grid and actor relations (Bitonic sort)
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

    // Setup grid and actor relation ranges
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
    kernels.initStepVariables(
      actorCount,
      memories.actorStates,
      memories.physicalQuantities,
      memories.constants
    );
    kernels.updateByPenaltyImpulse(
      actorCount,
      memories.actorStates,
      deltaTime
    );

    kernels.updateDensityAndPressure(
      fluidActorCount,
      memories.actorStates,
      memories.fluidStates,
      memories.constants
    );

    kernels.updateFluidForce(
      fluidActorCount,
      memories.actorStates,
      memories.fluidStates,
      memories.constants
    );

    for (int i = 0; i < constraintResolvingIterationCount; i++) {
      kernels.collectCollisions(
        softBodyActorCount,
        memories.actorStates
      );

      kernels.updateByConstraintImpulse(
        softBodyActorCount,
        memories.actorStates,
        memories.softBodyStates
      );
    }

    kernels.updateByFrictionalImpulse(
      softBodyActorCount,
      memories.actorStates
    );
  }

  void Simulator::motion() {
    kernels.moveFluid(
      fluidActorCount,
      memories.fluidStates,
      memories.actorStates,
      memories.physicalQuantities,
      memories.constants
    );

    for (int i = 0; i < motionIterationCount; i++) {
      if (springCount > 0) {
        kernels.calcSpringImpulses(
          springCount,
          memories.constants,
          memories.springStates,
          memories.physicalQuantities
        );
      }
      kernels.updateBySpringImpulse(
        softBodyActorCount,
        memories.constants,
        memories.softBodyStates,
        memories.actorStates,
        memories.physicalQuantities,
        memories.springStates
      );
    }

    kernels.postProcessing(
      softBodyActorCount,
      memories.constants,
      memories.actorStates,
      memories.physicalQuantities
    );
  }

  void Simulator::input() {
    setUpComputingSize();
    writeHostMemories();
    setUpMemories();
  }

  void Simulator::update() {
    computeBroadPhase();
    computeNarrowPhase();
    resolveConstraints();
    motion();
  }

  void Simulator::add(SoftBodyActor *actor) {
    softBodyActors.push_back(actor);
  }

  SoftBodyActor* Simulator::getSoftBodyActor(unsigned long i) {
    return softBodyActors[i];
  }

  void Simulator::add(Spring *spring) {
    springs.push_back(spring);
  }

  void Simulator::add(FluidActor *actor) {
    fluidActors.push_back(actor);
  }
}
