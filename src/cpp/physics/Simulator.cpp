#include "Simulator.h"

namespace alcube::physics {
  using namespace utils::opencl::kernelargs;
  Simulator::Simulator(
    utils::opencl::Resources *resources,
    utils::FileUtil* fileUtil,
    unsigned int maxActorCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount
  ) : SimulatorBase(
    resources,
    fileUtil,
    maxActorCount,
    maxActorCount * 16,
    xGridCount * yGridCount * zGridCount
  ) {
    softBodyParticles = {};
    springs = {};
    fluidParticles = {};
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    dtos.grid->edgeLength = gridEdgeLength;
    dtos.grid->xCount = xGridCount;
    dtos.grid->yCount = yGridCount;
    dtos.grid->zCount = zGridCount;
    dtos.grid->origin.s[0] = -(float)((xGridCount * gridEdgeLength) / 2);
    dtos.grid->origin.s[1] = -(float)((yGridCount * gridEdgeLength) / 2);
    dtos.grid->origin.s[2] = -(float)((zGridCount * gridEdgeLength) / 2);
    for (int i = 0; i < 3; i++) {
      dtos.grid->normals[i].s[0] = 0.0f;
      dtos.grid->normals[i].s[0] = 0.0f;
      dtos.grid->normals[i].s[0] = 0.0f;
      dtos.grid->normals[i].s[i] = 1.0f;
    }
    for (int i = 3; i < 6; i++) {
      dtos.grid->normals[i].s[0] = 0.0f;
      dtos.grid->normals[i].s[0] = 0.0f;
      dtos.grid->normals[i].s[0] = 0.0f;
      dtos.grid->normals[i].s[i - 3] = -1.0f;
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

    queue->push(kernels.inputConstants, {1}, {
      memArg(memories.constants),
      memArg(memories.grid),
      memArg(memories.fluidSettings),
      floatArg(gravity),
      floatArg(deltaTime),
      floatArg(splitDeltaTime),
      floatArg(sphericalShellRadius),
      ushortArg(rigidBodyParticleCountShort),
      ushortArg(fluidParticleCountShort)
    });

    queue->write(memories.inputFluidStates);
    queue->push(kernels.inputFluid, {fluidParticleCount}, {
      memArg(memories.inputFluidStates),
      memArg(memories.fluidStates)
    });
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

      opencl::dtos::FluidState* fluidState = &dtos.inputFluidStates[i];
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
    memories.actors->count = actorCount;
    memories.actorStates->count = actorCount;
    memories.currentStates->count = actorCount;
    memories.nextStates->count = actorCount;
    memories.gridAndActorRelations->count = actorCountForBitonicSort;
    memories.springs->count = springCount;
    memories.springVars->count = springCount;
    memories.fluidStates->count = fluidParticleCount;
    memories.inputFluidStates->count = fluidParticleCount;
    queue->write(memories.actors);
    queue->write(memories.currentStates);
    queue->write(memories.springs);
  }

  void Simulator::computeBroadPhase() {
    auto maxParticleCountShort = (unsigned short)maxActorCount;
    // Initialize grid and particle relations
    queue->push(kernels.initGridAndActorRelations, {actorCountForBitonicSort}, {
      memArg(memories.gridAndActorRelations),
      uintArg(allGridCount),
      ushortArg(maxParticleCountShort)
    });

    // Set grid index to rigid body state, and register grid and particle relations.
    queue->push(kernels.fillGridIndex, {actorCount}, {
      memArg(memories.grid),
      memArg(memories.actors),
      memArg(memories.actorStates),
      memArg(memories.currentStates),
      memArg(memories.nextStates),
      memArg(memories.gridAndActorRelations)
    });

    // Sort grid and particle relations (Bitonic sort)
    auto stageCount = (int)log2(actorCountForBitonicSort);
    int passCount = 0;
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
        queue->push(kernels.bitonic, {actorCountForBitonicSort}, {
          memArg(memories.gridAndActorRelations),
          uintArg(distance),
          uintArg(stageDistance)
        });
      }
      passCount++;
    }
    passCount = stageCount;
    for (int i = 0; i < passCount; i++) {
      auto distance = (unsigned int)(1 << (stageCount - (i + 1)));
      queue->push(kernels.merge, {actorCountForBitonicSort}, {
        memArg(memories.gridAndActorRelations),
        uintArg(distance)
      });
    }

    // Setup grid and particle relation ranges
    queue->pushZeroFill(memories.gridStartIndices);
    queue->pushZeroFill(memories.gridEndIndices);
    queue->push(kernels.setGridRelationIndexRange, {actorCount > 1 ? actorCount - 1 : 1}, {
      memArg(memories.gridAndActorRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      uintArg(actorCount)
    });
  }

  void Simulator::computeNarrowPhase() {
    queue->push(kernels.collectIntersections, {actorCount}, {
      memArg(memories.actorStates),
      memArg(memories.springs),
      memArg(memories.nextStates),
      memArg(memories.gridAndActorRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      memArg(memories.constants)
    });
  }

  void Simulator::resolveConstraints(float deltaTime) {
    queue->push(kernels.updateByPenaltyImpulse, {softBodyParticleCount}, {
      memArg(memories.actors),
      memArg(memories.actorStates),
      floatArg(deltaTime)
    });

    for (int i = 0; i < 16; i++) {
      queue->push(kernels.collectCollisions, {softBodyParticleCount}, {
        memArg(memories.actors),
        memArg(memories.actorStates)
      });
      queue->push(kernels.updateByConstraintImpulse, {softBodyParticleCount}, {
        memArg(memories.actors),
        memArg(memories.actorStates)
      });
    }
    queue->push(kernels.updateByFrictionalImpulse, {softBodyParticleCount}, {
      memArg(memories.actors),
      memArg(memories.actorStates)
    });
  }

  void Simulator::motion(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    for (int i = 0; i < splitCount; i++) {
      if (springCount > 0) {
        queue->push(kernels.calcSpringImpulses, {springCount}, {
          memArg(memories.actorStates),
          memArg(memories.springs),
          memArg(memories.springVars),
          memArg(memories.nextStates),
          floatArg(splitDeltaTime)
        });
      }
      queue->push(kernels.updateBySpringImpulse, {softBodyParticleCount}, {
        memArg(memories.actors),
        memArg(memories.actorStates),
        memArg(memories.nextStates),
        memArg(memories.springVars),
        floatArg(splitDeltaTime)
      });
    }
    queue->push(kernels.postProcessing, {softBodyParticleCount}, {
      memArg(memories.grid),
      memArg(memories.actors),
      memArg(memories.actorStates),
      memArg(memories.nextStates),
      floatArg(deltaTime)
    });
  }

  void Simulator::updateFluid(float deltaTime) {
    queue->push(kernels.updateDensityAndPressure, {fluidParticleCount}, {
      memArg(memories.actorStates),
      memArg(memories.fluidStates),
      memArg(memories.constants)
    });

    queue->push(kernels.updateFluidForce, {fluidParticleCount}, {
      memArg(memories.actorStates),
      memArg(memories.fluidStates),
      memArg(memories.constants)
    });

    queue->push(kernels.moveFluid, {fluidParticleCount}, {
      memArg(memories.fluidStates),
      memArg(memories.nextStates),
      memArg(memories.constants)
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
    read(memories.nextStates, dtos.nextStates);
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
