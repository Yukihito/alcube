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
  ) : SimulatorBase(
    resourcesProvider,
    maxActorCount,
    utils::math::powerOf2(maxActorCount),
    maxActorCount * 16,
    xGridCount * yGridCount * zGridCount
  ) {
    softBodyParticles = {};
    springs = {};
    fluidParticles = {};
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    auto grid = &memories.grid.at(0);
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

    dtos.fluidSettings->stiffness = 16.0f;
    dtos.fluidSettings->density = 0.1f;
    dtos.fluidSettings->viscosity = 8.0f;
    dtos.fluidSettings->particleMass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * dtos.fluidSettings->density;
    dtos.fluidSettings->effectiveRadius = 2.0f;
    dtos.fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(dtos.fluidSettings->effectiveRadius, 9));
    dtos.fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(dtos.fluidSettings->effectiveRadius, 6));
    dtos.fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(dtos.fluidSettings->effectiveRadius, 6));

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
    dtos.springs[springIndex].actorIndices[nodeIndex] = actorIndex;
    assignClFloat3(dtos.springs[springIndex].nodePositionsModelSpace[nodeIndex], springs[springIndex]->nodes[nodeIndex].position);
    opencl::dtos::Actor* actor = &dtos.actors[actorIndex];
    actor->springIndices[actor->springCount] = springIndex;
    actor->springNodeIndices[actor->springCount] = nodeIndex;
    actor->springCount++;
  }

  void Simulator::initGPUMemory(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    auto rigidBodyParticleCountShort = (unsigned short)softBodyParticleCount;
    auto fluidParticleCountShort = (unsigned short)fluidParticleCount;

    queue->push(_kernels.inputConstants, {1}, {
      memArg(memories._constants),
      memArg(memories._grid),
      memArg(memories._fluidSettings),
      floatArg(gravity),
      floatArg(deltaTime),
      floatArg(splitDeltaTime),
      floatArg(sphericalShellRadius),
      ushortArg(rigidBodyParticleCountShort),
      ushortArg(fluidParticleCountShort)
    });

    queue->write(memories._inputFluidStates);
    /*
    queue->push(_kernels.inputFluid, {fluidParticleCount}, {
      memArg(memories._inputFluidStates),
      memArg(memories._fluidStates)
    });
     */
    kernels.inputFluid(fluidParticleCount, memories.inputFluidStates, memories.fluidStates);
  }

  void Simulator::input() {

    glm::vec3 vec3Zero = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      dtos.actors[i].radius = softBodyParticle->radius;
      dtos.actors[i].mass = softBodyParticle->mass;
      dtos.actors[i].elasticity = softBodyParticle->elasticity;
      dtos.actors[i].staticFrictionCoefficient = softBodyParticle->staticFrictionCoefficient;
      dtos.actors[i].dynamicFrictionCoefficient = softBodyParticle->dynamicFrictionCoefficient;
      dtos.actors[i].radiusForAlterEgo = softBodyParticle->radiusForAlterEgo;
      dtos.actors[i].type = 0;
      assignClFloat3(dtos.currentStates[i].linearMomentum, softBodyParticle->linearMomentum);
      assignClFloat3(dtos.currentStates[i].angularMomentum, softBodyParticle->angularMomentum);
      assignClFloat3(dtos.currentStates[i].position, softBodyParticle->position);
      assignClFloat4(dtos.currentStates[i].rotation, softBodyParticle->rotation);
      softBodyParticle->index = (unsigned short)i;
      dtos.actors[i].springCount = 0;
    }
    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + softBodyParticleCount;
      FluidParticle* fluidParticle = fluidParticles[i];
      dtos.actors[globalIndex].radius = dtos.fluidSettings->effectiveRadius / 2.0f;
      dtos.actors[globalIndex].mass = dtos.fluidSettings->particleMass;
      dtos.actors[globalIndex].elasticity = 0.0f;
      dtos.actors[globalIndex].staticFrictionCoefficient = 0.0f;
      dtos.actors[globalIndex].dynamicFrictionCoefficient = 0.0f;
      dtos.actors[globalIndex].radiusForAlterEgo = 1.0f;
      dtos.actors[globalIndex].alterEgoIndex = -1;
      dtos.actors[globalIndex].type = 3;
      glm::quat quatIdent = glm::quat();
      assignClFloat3(dtos.currentStates[globalIndex].linearMomentum, vec3Zero);
      assignClFloat3(dtos.currentStates[globalIndex].angularMomentum, vec3Zero);
      assignClFloat3(dtos.currentStates[globalIndex].position, fluidParticle->position);
      assignClFloat4(dtos.currentStates[globalIndex].rotation, quatIdent);
      dtos.actors[i].springCount = 0;

      opencl::dtos::FluidState* fluidState = &memories.inputFluidStates.at(i);//&dtos.inputFluidStates[i];
      fluidState->density = 0.0f;
      assignClFloat3(fluidState->force, vec3Zero);
      fluidState->pressure = 0.0f;
      assignClFloat3(fluidState->velocity, vec3Zero);
    }
    for (unsigned int i = 0; i < springCount; i++) {
      dtos.springs[i].k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      if (softBodyParticle->alterEgo == nullptr) {
        dtos.actors[i].alterEgoIndex = -1;
      } else {
        dtos.actors[i].alterEgoIndex = softBodyParticle->alterEgo->index;
      }
    }
  }

  void Simulator::output() {
    for (int i = 0; i < softBodyParticleCount; i++) {
      SoftBodyParticle* softBodyParticle = softBodyParticles[i];
      assignGlmVec3(softBodyParticle->linearMomentum, dtos.nextStates[i].linearMomentum);
      assignGlmVec3(softBodyParticle->angularMomentum, dtos.nextStates[i].angularMomentum);
      assignGlmVec3(softBodyParticle->position, dtos.nextStates[i].position);
      assignGlmQuat(softBodyParticle->rotation, dtos.nextStates[i].rotation);
    }

    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + softBodyParticleCount;
      FluidParticle* particle = fluidParticles[i];
      assignGlmVec3(particle->position, dtos.nextStates[globalIndex].position);
    }
  }

  void Simulator::setUpMemories() {
    memories._actors->count = actorCount;
    memories._actorStates->count = actorCount;
    memories._currentStates->count = actorCount;
    memories._nextStates->count = actorCount;
    memories._gridAndActorRelations->count = actorCountForBitonicSort;
    memories._springs->count = springCount;
    memories._springVars->count = springCount;
    memories._fluidStates->count = fluidParticleCount;
    memories._inputFluidStates->count = fluidParticleCount;
    queue->write(memories._actors);
    queue->write(memories._currentStates);
    queue->write(memories._springs);
  }

  void Simulator::computeBroadPhase() {
    auto maxParticleCountShort = (unsigned short)maxActorCount;
    // Initialize grid and particle relations
    queue->push(_kernels.initGridAndActorRelations, {actorCountForBitonicSort}, {
      memArg(memories._gridAndActorRelations),
      uintArg(allGridCount),
      ushortArg(maxParticleCountShort)
    });

    // Set grid index to rigid body state, and register grid and particle relations.
    queue->push(_kernels.fillGridIndex, {actorCount}, {
      memArg(memories._grid),
      memArg(memories._actors),
      memArg(memories._actorStates),
      memArg(memories._currentStates),
      memArg(memories._nextStates),
      memArg(memories._gridAndActorRelations)
    });

    // Sort grid and particle relations (Bitonic sort)
    auto stageCount = (int)log2(actorCountForBitonicSort);
    int passCount = 0;
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
        queue->push(_kernels.bitonic, {actorCountForBitonicSort}, {
          memArg(memories._gridAndActorRelations),
          uintArg(distance),
          uintArg(stageDistance)
        });
      }
      passCount++;
    }
    passCount = stageCount;
    for (int i = 0; i < passCount; i++) {
      auto distance = (unsigned int)(1 << (stageCount - (i + 1)));
      queue->push(_kernels.merge, {actorCountForBitonicSort}, {
        memArg(memories._gridAndActorRelations),
        uintArg(distance)
      });
    }

    // Setup grid and particle relation ranges
    queue->pushZeroFill(memories._gridStartIndices);
    queue->pushZeroFill(memories._gridEndIndices);
    queue->push(_kernels.setGridRelationIndexRange, {actorCount > 1 ? actorCount - 1 : 1}, {
      memArg(memories._gridAndActorRelations),
      memArg(memories._gridStartIndices),
      memArg(memories._gridEndIndices),
      uintArg(actorCount)
    });
  }

  void Simulator::computeNarrowPhase() {
    queue->push(_kernels.collectIntersections, {actorCount}, {
      memArg(memories._actorStates),
      memArg(memories._springs),
      memArg(memories._nextStates),
      memArg(memories._gridAndActorRelations),
      memArg(memories._gridStartIndices),
      memArg(memories._gridEndIndices),
      memArg(memories._constants)
    });
  }

  void Simulator::resolveConstraints(float deltaTime) {
    queue->push(_kernels.updateByPenaltyImpulse, {softBodyParticleCount}, {
      memArg(memories._actors),
      memArg(memories._actorStates),
      floatArg(deltaTime)
    });

    for (int i = 0; i < 16; i++) {
      queue->push(_kernels.collectCollisions, {softBodyParticleCount}, {
        memArg(memories._actors),
        memArg(memories._actorStates)
      });
      queue->push(_kernels.updateByConstraintImpulse, {softBodyParticleCount}, {
        memArg(memories._actors),
        memArg(memories._actorStates)
      });
    }
    queue->push(_kernels.updateByFrictionalImpulse, {softBodyParticleCount}, {
      memArg(memories._actors),
      memArg(memories._actorStates)
    });
  }

  void Simulator::motion(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    for (int i = 0; i < splitCount; i++) {
      if (springCount > 0) {
        queue->push(_kernels.calcSpringImpulses, {springCount}, {
          memArg(memories._actorStates),
          memArg(memories._springs),
          memArg(memories._springVars),
          memArg(memories._nextStates),
          floatArg(splitDeltaTime)
        });
      }
      queue->push(_kernels.updateBySpringImpulse, {softBodyParticleCount}, {
        memArg(memories._actors),
        memArg(memories._actorStates),
        memArg(memories._nextStates),
        memArg(memories._springVars),
        floatArg(splitDeltaTime)
      });
    }
    queue->push(_kernels.postProcessing, {softBodyParticleCount}, {
      memArg(memories._grid),
      memArg(memories._actors),
      memArg(memories._actorStates),
      memArg(memories._nextStates),
      floatArg(deltaTime)
    });
  }

  void Simulator::updateFluid(float deltaTime) {
    queue->push(_kernels.updateDensityAndPressure, {fluidParticleCount}, {
      memArg(memories._actorStates),
      memArg(memories._fluidStates),
      memArg(memories._constants)
    });

    queue->push(_kernels.updateFluidForce, {fluidParticleCount}, {
      memArg(memories._actorStates),
      memArg(memories._fluidStates),
      memArg(memories._constants)
    });

    queue->push(_kernels.moveFluid, {fluidParticleCount}, {
      memArg(memories._fluidStates),
      memArg(memories._nextStates),
      memArg(memories._constants)
    });
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
    read(memories._nextStates, dtos.nextStates);
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
