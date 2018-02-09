#include "Simulator.h"

namespace alcube::physics {
  using namespace utils::opencl::kernelargs;
  Simulator::Simulator(
    utils::opencl::Resources *resources,
    utils::FileUtil* fileUtil,
    std::mutex* cellsMutex,
    unsigned int maxParticleCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount
  ) : utils::opencl::Simulator(resources, fileUtil) {
    this->cellsMutex = cellsMutex;
    this->maxParticleCount = maxParticleCount;
    this->maxParticleCountForBitonicSort = utils::math::powerOf2(maxParticleCount);
    this->maxSpringCount = maxParticleCount * 16;
    this->allGridCount = xGridCount * yGridCount * zGridCount;

    cells = {};
    springs = {};
    fluidParticles = {};
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    cl_program program = programFactory->create({
      "../src/kernels/physics/common.cl",
      "../src/kernels/physics/broadphase.cl",
      "../src/kernels/physics/narrowphase.cl",
      "../src/kernels/physics/constraintresolving.cl",
      "../src/kernels/physics/softbody.cl",
      "../src/kernels/physics/motion.cl",
      "../src/kernels/physics/fluid.cl"
    });
    kernels.fillGridIndex = kernelFactory->create(program, "fillGridIndex");
    kernels.merge = kernelFactory->create(program, "merge");
    kernels.bitonic = kernelFactory->create(program, "bitonic");
    kernels.setGridRelationIndexRange = kernelFactory->create(program, "setGridRelationIndexRange");
    kernels.initGridAndCellRelations = kernelFactory->create(program, "initGridAndCellRelations");
    kernels.collectIntersections = kernelFactory->create(program, "collectIntersections");
    kernels.updateByPenaltyImpulse = kernelFactory->create(program, "updateByPenaltyImpulse");
    kernels.updateByFrictionalImpulse = kernelFactory->create(program, "updateByFrictionalImpulse");
    kernels.collectCollisions = kernelFactory->create(program, "collectCollisions");
    kernels.updateByConstraintImpulse = kernelFactory->create(program, "updateByConstraintImpulse");
    kernels.calcSpringImpulses = kernelFactory->create(program, "calcSpringImpulses");
    kernels.updateBySpringImpulse = kernelFactory->create(program, "updateBySpringImpulse");
    kernels.postProcessing = kernelFactory->create(program, "postProcessing");
    kernels.inputFluid = kernelFactory->create(program, "inputFluid");
    kernels.updateDensityAndPressure = kernelFactory->create(program, "updateDensityAndPressure");
    kernels.updateFluidForce = kernelFactory->create(program, "updateFluidForce");
    kernels.moveFluid = kernelFactory->create(program, "moveFluid");
    kernels.inputConstants = kernelFactory->create(program, "inputConstants");

    dtos.grid = new opencl::dtos::Grid();
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
    dtos.cells = new opencl::dtos::Cell[maxParticleCount];
    dtos.cellVars = new opencl::dtos::CellVar[maxParticleCount];
    dtos.currentStates = new opencl::dtos::RigidBodyState[maxParticleCount];
    dtos.nextStates = new opencl::dtos::RigidBodyState[maxParticleCount];
    dtos.gridAndCellRelations = new opencl::dtos::GridAndCellRelation[maxParticleCount];
    dtos.fluidStates = new opencl::dtos::FluidState[maxParticleCount];
    dtos.inputFluidStates = new opencl::dtos::FluidState[maxParticleCount];
    dtos.gridStartIndices = new unsigned int[allGridCount];
    dtos.gridEndIndices = new unsigned int[allGridCount];
    dtos.springs = new opencl::dtos::Spring[maxSpringCount];

    dtos.fluidSettings = new opencl::dtos::FluidSettings();
    dtos.fluidSettings->stiffness = 16.0f;
    dtos.fluidSettings->density = 0.1f;
    dtos.fluidSettings->viscosity = 8.0f;
    dtos.fluidSettings->particleMass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * dtos.fluidSettings->density;
    dtos.fluidSettings->effectiveRadius = 2.0f;
    dtos.fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(dtos.fluidSettings->effectiveRadius, 9));
    dtos.fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(dtos.fluidSettings->effectiveRadius, 6));
    dtos.fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(dtos.fluidSettings->effectiveRadius, 6));

    memories.grid = memoryManager->define("grid", sizeof(opencl::dtos::Grid), dtos.grid, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 1, 1);
    memories.cells = memoryManager->define("cells", sizeof(opencl::dtos::Cell), dtos.cells, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxParticleCount, 0);
    memories.currentStates = memoryManager->define("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxParticleCount, 0);
    memories.cellVars = memoryManager->define("cellVars", sizeof(opencl::dtos::CellVar), nullptr, CL_MEM_READ_WRITE, maxParticleCount, 0);
    memories.nextStates = memoryManager->define("nextStates", sizeof(opencl::dtos::RigidBodyState), nullptr, CL_MEM_READ_WRITE, maxParticleCount, 0);
    memories.gridAndCellRelations = memoryManager->define("gridAndCellRelations", sizeof(opencl::dtos::GridAndCellRelation), nullptr, CL_MEM_READ_WRITE, maxParticleCountForBitonicSort, 0);
    memories.gridStartIndices = memoryManager->define("gridStartIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE, allGridCount, allGridCount);
    memories.gridEndIndices = memoryManager->define("gridEndIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE, allGridCount, allGridCount);
    memories.springs = memoryManager->define("springs", sizeof(opencl::dtos::Spring), dtos.springs, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxSpringCount, 0);
    memories.springVars = memoryManager->define("springVars", sizeof(opencl::dtos::SpringVar), nullptr, CL_MEM_READ_WRITE, maxSpringCount, 0);
    memories.inputFluidStates = memoryManager->define("inputFluidStates", sizeof(opencl::dtos::FluidState), dtos.inputFluidStates, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxParticleCount, 0);
    memories.fluidStates = memoryManager->define("fluidStates", sizeof(opencl::dtos::FluidState), nullptr, CL_MEM_READ_WRITE, maxParticleCount, 0);
    memories.fluidSettings = memoryManager->define("fluidSettings", sizeof(opencl::dtos::FluidSettings), dtos.fluidSettings, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 1, 1);
    memories.constants = memoryManager->define("constants", sizeof(opencl::dtos::Constants), nullptr, CL_MEM_READ_WRITE, 1, 1);
    memoryManager->allocate();
    initialized = false;
  }

  void Simulator::setUpComputingSize() {
    rigidBodyParticleCount = (unsigned int)cells.size();
    fluidParticleCount = (unsigned int)fluidParticles.size();
    particleCount = rigidBodyParticleCount + fluidParticleCount;
    springCount = (unsigned int)springs.size();
    particleCountForBitonicSort = utils::math::powerOf2(particleCount);
  }

  void Simulator::setUpSpring(unsigned int springIndex, unsigned char nodeIndex) {
    unsigned short cellIndex = springs[springIndex]->nodes[nodeIndex].cell->index;
    dtos.springs[springIndex].cellIndices[nodeIndex] = cellIndex;
    assignClFloat3(dtos.springs[springIndex].nodePositionsModelSpace[nodeIndex], springs[springIndex]->nodes[nodeIndex].position);
    opencl::dtos::Cell* cell = &dtos.cells[cellIndex];
    cell->springIndices[cell->springCount] = springIndex;
    cell->springNodeIndices[cell->springCount] = nodeIndex;
    cell->springCount++;
  }

  void Simulator::initGPUMemory(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    auto rigidBodyParticleCountShort = (unsigned short)rigidBodyParticleCount;
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
    for (int i = 0; i < rigidBodyParticleCount; i++) {
      Cell* cell = cells[i];
      dtos.cells[i].radius = cell->radius;
      dtos.cells[i].mass = cell->mass;
      dtos.cells[i].elasticity = cell->elasticity;
      dtos.cells[i].staticFrictionCoefficient = cell->staticFrictionCoefficient;
      dtos.cells[i].dynamicFrictionCoefficient = cell->dynamicFrictionCoefficient;
      dtos.cells[i].radiusForAlterEgo = cell->radiusForAlterEgo;
      dtos.cells[i].type = 0;
      assignClFloat3(dtos.currentStates[i].linearMomentum, cell->linearMomentum);
      assignClFloat3(dtos.currentStates[i].angularMomentum, cell->angularMomentum);
      assignClFloat3(dtos.currentStates[i].position, cell->position);
      assignClFloat4(dtos.currentStates[i].rotation, cell->rotation);
      cell->index = (unsigned short)i;
      dtos.cells[i].springCount = 0;
    }
    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + rigidBodyParticleCount;
      FluidParticle* fluidParticle = fluidParticles[i];
      dtos.cells[globalIndex].radius = dtos.fluidSettings->effectiveRadius / 2.0f;
      dtos.cells[globalIndex].mass = dtos.fluidSettings->particleMass;
      dtos.cells[globalIndex].elasticity = 0.0f;
      dtos.cells[globalIndex].staticFrictionCoefficient = 0.0f;
      dtos.cells[globalIndex].dynamicFrictionCoefficient = 0.0f;
      dtos.cells[globalIndex].radiusForAlterEgo = 1.0f;
      dtos.cells[globalIndex].alterEgoIndex = -1;
      dtos.cells[globalIndex].type = 3;
      glm::quat quatIdent = glm::quat();
      assignClFloat3(dtos.currentStates[globalIndex].linearMomentum, vec3Zero);
      assignClFloat3(dtos.currentStates[globalIndex].angularMomentum, vec3Zero);
      assignClFloat3(dtos.currentStates[globalIndex].position, fluidParticle->position);
      assignClFloat4(dtos.currentStates[globalIndex].rotation, quatIdent);
      dtos.cells[i].springCount = 0;

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
    for (int i = 0; i < rigidBodyParticleCount; i++) {
      Cell* cell = cells[i];
      if (cell->alterEgo == nullptr) {
        dtos.cells[i].alterEgoIndex = -1;
      } else {
        dtos.cells[i].alterEgoIndex = cell->alterEgo->index;
      }
    }
  }

  void Simulator::output() {
    for (int i = 0; i < rigidBodyParticleCount; i++) {
      Cell* cell = cells[i];
      assignGlmVec3(cell->linearMomentum, dtos.nextStates[i].linearMomentum);
      assignGlmVec3(cell->angularMomentum, dtos.nextStates[i].angularMomentum);
      assignGlmVec3(cell->position, dtos.nextStates[i].position);
      assignGlmQuat(cell->rotation, dtos.nextStates[i].rotation);
    }

    for (int i = 0; i < fluidParticleCount; i++) {
      int globalIndex = i + rigidBodyParticleCount;
      //Cell* cell = cells[globalIndex];
      FluidParticle* particle = fluidParticles[i];
      //assignGlmVec3(cell->linearMomentum, dtos.nextStates[globalIndex].linearMomentum);
      //assignGlmVec3(cell->angularMomentum, dtos.nextStates[globalIndex].angularMomentum);
      assignGlmVec3(particle->position, dtos.nextStates[globalIndex].position);
      //assignGlmQuat(cell->rotation, dtos.nextStates[globalIndex].rotation);
    }
  }

  void Simulator::setUpMemories() {
    memories.cells->count = particleCount;
    memories.cellVars->count = particleCount;
    memories.currentStates->count = particleCount;
    memories.nextStates->count = particleCount;
    memories.gridAndCellRelations->count = particleCountForBitonicSort;
    memories.springs->count = springCount;
    memories.springVars->count = springCount;
    memories.fluidStates->count = fluidParticleCount;
    memories.inputFluidStates->count = fluidParticleCount;
    queue->write(memories.cells);
    queue->write(memories.currentStates);
    queue->write(memories.springs);
  }

  void Simulator::computeBroadPhase() {
    auto maxParticleCountShort = (unsigned short)maxParticleCount;
    // Initialize grid and cell relations
    queue->push(kernels.initGridAndCellRelations, {particleCountForBitonicSort}, {
      memArg(memories.gridAndCellRelations),
      uintArg(allGridCount),
      ushortArg(maxParticleCountShort)
    });

    // Set grid index to rigid body state, and register grid and cell relations.
    queue->push(kernels.fillGridIndex, {particleCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.currentStates),
      memArg(memories.nextStates),
      memArg(memories.gridAndCellRelations)
    });

    // Sort grid and cell relations (Bitonic sort)
    auto stageCount = (int)log2(particleCountForBitonicSort);
    int passCount = 0;
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
        queue->push(kernels.bitonic, {particleCountForBitonicSort}, {
          memArg(memories.gridAndCellRelations),
          uintArg(distance),
          uintArg(stageDistance)
        });
      }
      passCount++;
    }
    passCount = stageCount;
    for (int i = 0; i < passCount; i++) {
      auto distance = (unsigned int)(1 << (stageCount - (i + 1)));
      queue->push(kernels.merge, {particleCountForBitonicSort}, {
        memArg(memories.gridAndCellRelations),
        uintArg(distance)
      });
    }

    // Setup grid and cell relation ranges
    queue->pushZeroFill(memories.gridStartIndices);
    queue->pushZeroFill(memories.gridEndIndices);
    queue->push(kernels.setGridRelationIndexRange, {particleCount > 1 ? particleCount - 1 : 1}, {
      memArg(memories.gridAndCellRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      uintArg(particleCount)
    });
  }

  void Simulator::computeNarrowPhase() {
    queue->push(kernels.collectIntersections, {particleCount}, {
      memArg(memories.cellVars),
      memArg(memories.springs),
      memArg(memories.nextStates),
      memArg(memories.gridAndCellRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      memArg(memories.constants)
    });
  }

  void Simulator::resolveConstraints(float deltaTime) {
    queue->push(kernels.updateByPenaltyImpulse, {rigidBodyParticleCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars),
      floatArg(deltaTime)
    });

    for (int i = 0; i < 16; i++) {
      queue->push(kernels.collectCollisions, {rigidBodyParticleCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars)
      });
      queue->push(kernels.updateByConstraintImpulse, {rigidBodyParticleCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars)
      });
    }
    queue->push(kernels.updateByFrictionalImpulse, {rigidBodyParticleCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars)
    });
  }

  void Simulator::motion(float deltaTime) {
    int splitCount = 8;
    float splitDeltaTime = deltaTime / splitCount;
    for (int i = 0; i < splitCount; i++) {
      if (springCount > 0) {
        queue->push(kernels.calcSpringImpulses, {springCount}, {
          memArg(memories.cellVars),
          memArg(memories.springs),
          memArg(memories.springVars),
          memArg(memories.nextStates),
          floatArg(splitDeltaTime)
        });
      }
      queue->push(kernels.updateBySpringImpulse, {rigidBodyParticleCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars),
        memArg(memories.nextStates),
        memArg(memories.springVars),
        floatArg(splitDeltaTime)
      });
    }
    queue->push(kernels.postProcessing, {rigidBodyParticleCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.nextStates),
      floatArg(deltaTime)
    });
  }

  void Simulator::updateFluid(float deltaTime) {
    queue->push(kernels.updateDensityAndPressure, {fluidParticleCount}, {
      memArg(memories.cellVars),
      memArg(memories.fluidStates),
      memArg(memories.constants)
    });

    queue->push(kernels.updateFluidForce, {fluidParticleCount}, {
      memArg(memories.cellVars),
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

    cellsMutex->lock();
    output();
    cellsMutex->unlock();
  }

  void Simulator::add(Cell *cell) {
    cells.push_back(cell);
  }

  Cell* Simulator::getCell(unsigned long i) {
    return cells[i];
  }

  void Simulator::add(Spring *spring) {
    springs.push_back(spring);
  }

  void Simulator::add(FluidParticle *fluidParticle) {
    fluidParticles.push_back(fluidParticle);
  }
}
