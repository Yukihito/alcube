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
  ) {
    this->cellsMutex = cellsMutex;
    this->maxCellCount = maxCellCount;
    this->allGridCount = xGridCount * yGridCount * zGridCount;
    kernelFactory = new utils::opencl::KernelFactory(resources, fileUtil);
    programFactory = new utils::opencl::ProgramFactory(resources, fileUtil);
    memoryManager = new utils::opencl::MemoryManager(resources);
    queue = new utils::opencl::CommandQueue(resources);
    cells = {};
    gravity = 0.0f;

    cl_program program = programFactory->create("../src/kernels/physics/physics.cl");
    kernels.fillGridIndex = kernelFactory->create(program, "fillGridIndex");
    kernels.merge = kernelFactory->create(program, "merge");
    kernels.bitonic = kernelFactory->create(program, "bitonic");
    kernels.setGridRelationIndexRange = kernelFactory->create(program, "setGridRelationIndexRange");
    kernels.initGridAndCellRelations = kernelFactory->create(program, "initGridAndCellRelations");
    kernels.collectIntersections = kernelFactory->create(program, "collectIntersections");
    kernels.countIntersections = kernelFactory->create(program, "countIntersections");
    kernels.setUpIntersectionRefs = kernelFactory->create(program, "setUpIntersectionRefs");
    kernels.calcPenaltyImpulse = kernelFactory->create(program, "calcPenaltyImpulse");
    kernels.updateByPenaltyImpulse = kernelFactory->create(program, "updateByPenaltyImpulse");
    kernels.calcFrictionalImpulse = kernelFactory->create(program, "calcFrictionalImpulse");
    kernels.updateByFrictionalImpulse = kernelFactory->create(program, "updateByFrictionalImpulse");
    kernels.collectCollisions = kernelFactory->create(program, "collectCollisions");
    kernels.calcConstraintImpulse = kernelFactory->create(program, "calcConstraintImpulse");
    kernels.updateByConstraintImpulse = kernelFactory->create(program, "updateByConstraintImpulse");
    kernels.motion = kernelFactory->create(program, "motion");
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
    dtos.lastBlock = new opencl::dtos::IntersectionBlock[1];

    memories.grid = memoryManager->define("grid", sizeof(opencl::dtos::Grid), dtos.grid, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR);
    memories.cells = memoryManager->define("cells", sizeof(opencl::dtos::Cell), dtos.cells, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR);
    memories.currentStates = memoryManager->define("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR);
    memories.cellVars = memoryManager->define("cellVars", sizeof(opencl::dtos::CellVar), nullptr, CL_MEM_READ_WRITE);
    memories.intersections = memoryManager->define("intersections", sizeof(opencl::dtos::Intersection), nullptr, CL_MEM_READ_WRITE);
    memories.blocks = memoryManager->define("blocks", sizeof(opencl::dtos::IntersectionBlock), nullptr, CL_MEM_READ_WRITE);
    memories.intersectionRefs = memoryManager->define("intersectionRefs", sizeof(opencl::dtos::IntersectionRef), nullptr, CL_MEM_READ_WRITE);
    memories.nextStates = memoryManager->define("nextStates", sizeof(opencl::dtos::RigidBodyState), nullptr, CL_MEM_READ_WRITE);
    memories.gridAndCellRelations = memoryManager->define("gridAndCellRelations", sizeof(opencl::dtos::GridAndCellRelation), nullptr, CL_MEM_READ_WRITE);
    memories.gridStartIndices = memoryManager->define("gridStartIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE);
    memories.gridEndIndices = memoryManager->define("gridEndIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE);
  }

  void Simulator::setUpComputingSize() {
    cellCount = (unsigned int)cells.size();
    cellCountForBitonicSort = utils::math::powerOf2(cellCount);
  }

  void Simulator::input() {
    for (int i = 0; i < cellCount; i++) {
      Cell* cell = cells[i];
      dtos.cells[i].radius = cell->radius;
      dtos.cells[i].mass = cell->mass;
      dtos.cells[i].elasticity = cell->elasticity;
      dtos.cells[i].staticFrictionCoefficient = cell->staticFrictionCoefficient;
      dtos.cells[i].dynamicFrictionCoefficient = cell->dynamicFrictionCoefficient;
      dtos.currentStates[i].gridIndex = 0;
      assignClFloat3(dtos.currentStates[i].linearMomentum, cell->linearMomentum);
      assignClFloat3(dtos.currentStates[i].angularMomentum, cell->angularMomentum);
      assignClFloat3(dtos.currentStates[i].position, cell->position);
      assignClFloat4(dtos.currentStates[i].rotation, cell->rotation);
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
    memories.gridStartIndices->count = allGridCount;
    memories.gridEndIndices->count = allGridCount;
    memories.blocks->count = cellCount;
    memories.intersections->count = cellCount * 16;
    memories.intersectionRefs->count = cellCount * 16;
    memoryManager->allocate();
  }

  void Simulator::tearDownMemories() {
    memoryManager->release();
  }

  void Simulator::computeBroadPhase() {
    // Initialize grid and cell relations
    queue->push(kernels.initGridAndCellRelations, {cellCountForBitonicSort}, {
      memArg(memories.gridAndCellRelations),
      uintArg(allGridCount),
      uintArg(maxCellCount)
    });

    // Set grid index to rigid body state, and register grid and cell relations.
    queue->push(kernels.fillGridIndex, {cellCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.currentStates),
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
      memArg(memories.currentStates),
      memArg(memories.gridAndCellRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices),
      memArg(memories.blocks),
      floatArg(deltaTime),
      floatArg(gravity)
    });
  }

  void Simulator::resolveConstraints(float deltaTime) {
    for (unsigned int dist = 2; dist <= cellCount; dist *= 2) {
      unsigned int halfDist = dist / 2;
      queue->push(kernels.countIntersections, {cellCount}, {
        memArg(memories.blocks),
        uintArg(dist),
        uintArg(halfDist)
      });
    }

    queue->push(kernels.setUpIntersectionRefs, {cellCount}, {
      memArg(memories.blocks),
      memArg(memories.intersectionRefs)
    });

    queue->readAt(memories.blocks, dtos.lastBlock, cellCount - 1);
    //std::cout << "intersectionCount: " << dtos.lastBlock->cumulativeIntersectionCount << std::endl;
    if (dtos.lastBlock->cumulativeIntersectionCount == 0) {
      return;
    }
    queue->push(kernels.calcPenaltyImpulse, {dtos.lastBlock->cumulativeIntersectionCount}, {
      memArg(memories.intersectionRefs),
      memArg(memories.blocks),
      floatArg(deltaTime)
    });
    queue->push(kernels.updateByPenaltyImpulse, {cellCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.blocks)
    });

    for (int i = 0; i < 16; i++) {
      queue->push(kernels.collectCollisions, {cellCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars),
        memArg(memories.blocks)
      });
      queue->push(kernels.calcConstraintImpulse, {dtos.lastBlock->cumulativeIntersectionCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars),
        memArg(memories.intersectionRefs),
        memArg(memories.blocks)
      });
      queue->push(kernels.updateByConstraintImpulse, {cellCount}, {
        memArg(memories.cells),
        memArg(memories.cellVars),
        memArg(memories.blocks)
      });
    }

    queue->push(kernels.calcFrictionalImpulse, {dtos.lastBlock->cumulativeIntersectionCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.intersectionRefs),
      memArg(memories.blocks)
    });
    queue->push(kernels.updateByFrictionalImpulse, {cellCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.blocks)
    });
  }

  void Simulator::motion(float deltaTime) {
    queue->push(kernels.motion, {cellCount}, {
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.currentStates),
      memArg(memories.nextStates),
      floatArg(deltaTime)
    });

    queue->push(kernels.postProcessing, {cellCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.cellVars),
      memArg(memories.nextStates)
    });
  }

  void Simulator::read(utils::opencl::Memory *memory, void *hostPtr) {
    queue->read(memory, hostPtr);
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
    tearDownMemories();
    cellsMutex->lock();
    output();
    cellsMutex->unlock();
  }

  void Simulator::add(Cell *cell) {
    cells.push_back(cell);
  }
}
