#include "Simulator.h"

namespace alcube::physics {
  using namespace utils::opencl::kernelargs;
  Simulator::Simulator(
    utils::opencl::Resources *resources,
    utils::FileUtil* fileUtil,
    std::mutex* cellsMutex,
    unsigned int maxCellCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount
  ) : utils::opencl::Simulator(resources, fileUtil) {
    this->cellsMutex = cellsMutex;
    this->maxCellCount = maxCellCount;
    this->maxCellCountForBitonicSort = utils::math::powerOf2(maxCellCount);
    this->maxSpringCount = maxCellCount * 16;
    this->allGridCount = xGridCount * yGridCount * zGridCount;

    cells = {};
    springs = {};
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    cl_program program = programFactory->create({
      "../src/kernels/physics/common.cl",
      "../src/kernels/physics/broadphase.cl",
      "../src/kernels/physics/narrowphase.cl",
      "../src/kernels/physics/constraintresolving.cl",
      "../src/kernels/physics/softbody.cl",
      "../src/kernels/physics/motion.cl"
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
    dtos.cells = new opencl::dtos::Cell[maxCellCount];
    dtos.cellVars = new opencl::dtos::CellVar[maxCellCount];
    dtos.currentStates = new opencl::dtos::RigidBodyState[maxCellCount];
    dtos.nextStates = new opencl::dtos::RigidBodyState[maxCellCount];
    dtos.gridAndCellRelations = new opencl::dtos::GridAndCellRelation[maxCellCount];
    dtos.gridStartIndices = new unsigned int[allGridCount];
    dtos.gridEndIndices = new unsigned int[allGridCount];
    dtos.springs = new opencl::dtos::Spring[maxSpringCount];

    memories.grid = memoryManager->define("grid", sizeof(opencl::dtos::Grid), dtos.grid, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 1, 1);
    memories.cells = memoryManager->define("cells", sizeof(opencl::dtos::Cell), dtos.cells, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxCellCount, 0);
    memories.currentStates = memoryManager->define("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxCellCount, 0);
    memories.cellVars = memoryManager->define("cellVars", sizeof(opencl::dtos::CellVar), nullptr, CL_MEM_READ_WRITE, maxCellCount, 0);
    memories.nextStates = memoryManager->define("nextStates", sizeof(opencl::dtos::RigidBodyState), nullptr, CL_MEM_READ_WRITE, maxCellCount, 0);
    memories.gridAndCellRelations = memoryManager->define("gridAndCellRelations", sizeof(opencl::dtos::GridAndCellRelation), nullptr, CL_MEM_READ_WRITE, maxCellCountForBitonicSort, 0);
    memories.gridStartIndices = memoryManager->define("gridStartIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE, allGridCount, allGridCount);
    memories.gridEndIndices = memoryManager->define("gridEndIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE, allGridCount, allGridCount);
    memories.springs = memoryManager->define("springs", sizeof(opencl::dtos::Spring), dtos.springs, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxSpringCount, 0);
    memories.springVars = memoryManager->define("springVars", sizeof(opencl::dtos::SpringVar), nullptr, CL_MEM_READ_WRITE, maxSpringCount, 0);
    memoryManager->allocate();
  }

  void Simulator::setUpComputingSize() {
    cellCount = (unsigned int)cells.size();
    springCount = (unsigned int)springs.size();
    cellCountForBitonicSort = utils::math::powerOf2(cellCount);
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

  void Simulator::input() {
    for (int i = 0; i < cellCount; i++) {
      Cell* cell = cells[i];
      dtos.cells[i].radius = cell->radius;
      dtos.cells[i].mass = cell->mass;
      dtos.cells[i].elasticity = cell->elasticity;
      dtos.cells[i].staticFrictionCoefficient = cell->staticFrictionCoefficient;
      dtos.cells[i].dynamicFrictionCoefficient = cell->dynamicFrictionCoefficient;
      dtos.cells[i].radiusForAlterEgo = cell->radiusForAlterEgo;
      assignClFloat3(dtos.currentStates[i].linearMomentum, cell->linearMomentum);
      assignClFloat3(dtos.currentStates[i].angularMomentum, cell->angularMomentum);
      assignClFloat3(dtos.currentStates[i].position, cell->position);
      assignClFloat4(dtos.currentStates[i].rotation, cell->rotation);
      cell->index = (unsigned short)i;
      dtos.cells[i].springCount = 0;
    }
    for (unsigned int i = 0; i < springCount; i++) {
      dtos.springs[i].k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }
    for (int i = 0; i < cellCount; i++) {
      Cell* cell = cells[i];
      if (cell->alterEgo == nullptr) {
        dtos.cells[i].alterEgoIndex = -1;
      } else {
        dtos.cells[i].alterEgoIndex = cell->alterEgo->index;
      }
    }
  }

  void Simulator::output() {
    for (int i = 0; i < cellCount; i++) {
      Cell* cell = cells[i];
      assignGlmVec3(cell->linearMomentum, dtos.nextStates[i].linearMomentum);
      assignGlmVec3(cell->angularMomentum, dtos.nextStates[i].angularMomentum);
      assignGlmVec3(cell->position, dtos.nextStates[i].position);
      assignGlmQuat(cell->rotation, dtos.nextStates[i].rotation);
    }
  }

  void Simulator::setUpMemories() {
    memories.cells->count = cellCount;
    memories.cellVars->count = cellCount;
    memories.currentStates->count = cellCount;
    memories.nextStates->count = cellCount;
    memories.gridAndCellRelations->count = cellCountForBitonicSort;
    memories.springs->count = springCount;
    memories.springVars->count = springCount;
    queue->write(memories.cells);
    queue->write(memories.currentStates);
    queue->write(memories.springs);
  }

  void Simulator::computeBroadPhase() {
    auto maxCellCountShort = (unsigned short)maxCellCount;
    // Initialize grid and cell relations
    queue->push(kernels.initGridAndCellRelations, {cellCountForBitonicSort}, {
      memArg(memories.gridAndCellRelations),
      uintArg(allGridCount),
      ushortArg(maxCellCountShort)
    });

    // Set grid index to rigid body state, and register grid and cell relations.
    queue->push(kernels.fillGridIndex, {cellCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.currentStates),
      memArg(memories.nextStates),
      memArg(memories.gridAndCellRelations)
    });

    // Sort grid and cell relations (Bitonic sort)
    auto stageCount = (int)log2(cellCountForBitonicSort);
    int passCount = 0;
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
        queue->push(kernels.bitonic, {cellCountForBitonicSort}, {
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
      queue->push(kernels.merge, {cellCountForBitonicSort}, {
        memArg(memories.gridAndCellRelations),
        uintArg(distance)
      });
    }

    // Setup grid and cell relation ranges
    queue->pushZeroFill(memories.gridStartIndices);
    queue->pushZeroFill(memories.gridEndIndices);
    queue->push(kernels.setGridRelationIndexRange, {cellCount > 1 ? cellCount - 1 : 1}, {
      memArg(memories.gridAndCellRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      uintArg(cellCount)
    });
  }

  void Simulator::computeNarrowPhase(float deltaTime) {
    queue->push(kernels.collectIntersections, {cellCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.springs),
      memArg(memories.nextStates),
      memArg(memories.gridAndCellRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      floatArg(sphericalShellRadius),
      floatArg(deltaTime),
      floatArg(gravity)
    });
  }

  void Simulator::resolveConstraints(float deltaTime) {
    queue->push(kernels.updateByPenaltyImpulse, {cellCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars),
      floatArg(deltaTime)
    });

    for (int i = 0; i < 16; i++) {
      queue->push(kernels.collectCollisions, {cellCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars)
      });
      queue->push(kernels.updateByConstraintImpulse, {cellCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars)
      });
    }
    queue->push(kernels.updateByFrictionalImpulse, {cellCount}, {
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
      queue->push(kernels.updateBySpringImpulse, {cellCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars),
        memArg(memories.nextStates),
        memArg(memories.springVars),
        floatArg(splitDeltaTime)
      });
    }
    queue->push(kernels.postProcessing, {cellCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.nextStates),
      floatArg(deltaTime)
    });
  }

  void Simulator::update(float deltaTime) {
    setUpComputingSize();
    input();
    setUpMemories();

    computeBroadPhase();
    computeNarrowPhase(deltaTime);
    resolveConstraints(deltaTime);
    motion(deltaTime);
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
}
