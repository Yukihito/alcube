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

    cl_program setupGridProgram = programFactory->create("../src/kernels/physics/setupgrid.cl");
    kernels.fillGridIndex = kernelFactory->create(setupGridProgram, "fillGridIndex");
    kernels.merge = kernelFactory->create(setupGridProgram, "merge");
    kernels.bitonic = kernelFactory->create(setupGridProgram, "bitonic");
    kernels.setGridRelationIndexRange = kernelFactory->create(setupGridProgram, "setGridRelationIndexRange");
    kernels.initGridAndCellRelations = kernelFactory->create(setupGridProgram, "initGridAndCellRelations");

    dtos.grid = new opencl::dtos::Grid();
    dtos.grid->edgeLength = gridEdgeLength;
    dtos.grid->xCount = xGridCount;
    dtos.grid->yCount = yGridCount;
    dtos.grid->zCount = zGridCount;
    dtos.grid->origin.s[0] = -(float)((xGridCount * gridEdgeLength) / 2);
    dtos.grid->origin.s[1] = -(float)((yGridCount * gridEdgeLength) / 2);
    dtos.grid->origin.s[2] = -(float)((zGridCount * gridEdgeLength) / 2);
    dtos.cells = new opencl::dtos::Cell[maxCellCount];
    dtos.currentStates = new opencl::dtos::RigidBodyState[maxCellCount];
    dtos.gridAndCellRelations = new opencl::dtos::GridAndCellRelation[maxCellCount];

    memories.grid = memoryManager->define("grid", sizeof(opencl::dtos::Grid), dtos.grid, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR);
    memories.cells = memoryManager->define("cells", sizeof(opencl::dtos::Cell), dtos.cells, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR);
    memories.currentStates = memoryManager->define("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR);
    memories.gridAndCellRelations = memoryManager->define("gridAndCellRelations", sizeof(opencl::dtos::GridAndCellRelation), nullptr, CL_MEM_READ_WRITE);
    memories.gridStartIndices = memoryManager->define("gridStartIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE);
    memories.gridEndIndices = memoryManager->define("gridEndIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE);
  }

  void Simulator::update(float deltaTime) {
    auto cellCount = (unsigned int)cells.size();
    unsigned int cellCountForBitonicSort = utils::math::powerOf2(cellCount);

    // Model -> DTO
    for (int i = 0; i < cellCount; i++) {
      Cell* cell = cells[i];
      dtos.cells[i].radius = cell->radius;
      dtos.cells[i].weight = cell->weight;
      dtos.cells[i].springEndIndex = 0;
      dtos.cells[i].springStartIndex = 0;
      dtos.currentStates[i].gridIndex = 0;
      assignClFloat3(dtos.currentStates[i].linearMomentum, cell->currentState.linearMomentum);
      assignClFloat3(dtos.currentStates[i].angularMomentum, cell->currentState.angularMomentum);
      assignClFloat3(dtos.currentStates[i].position, cell->currentState.position);
      assignClFloat4(dtos.currentStates[i].rotation, cell->currentState.rotation);
    }

    // Set count of memory, and allocate
    memories.cells->count = cellCount;
    memories.currentStates->count = cellCount;
    memories.gridAndCellRelations->count = cellCountForBitonicSort;
    memories.gridStartIndices->count = allGridCount;
    memories.gridEndIndices->count = allGridCount;
    memoryManager->allocate();

    // Computing start
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
    queue->push(kernels.setGridRelationIndexRange, {cellCount - 1}, {
      memArg(memories.gridAndCellRelations),
      memArg(memories.gridStartIndices),
      memArg(memories.gridEndIndices)
    });

    // Read
    queue->read(memories.currentStates, dtos.currentStates);
    queue->read(memories.gridAndCellRelations, dtos.gridAndCellRelations);
    // Computing finish

    memoryManager->release();

    // DTO -> Model
    for (int i = 0; i < cellCount; i++) {
      Cell* cell = cells[i];
      assignGlmVec3(cell->currentState.linearMomentum, dtos.currentStates[i].linearMomentum);
      assignGlmVec3(cell->currentState.angularMomentum, dtos.currentStates[i].angularMomentum);
      assignGlmVec3(cell->currentState.position, dtos.currentStates[i].position);
      assignGlmQuat(cell->currentState.rotation, dtos.currentStates[i].rotation);
    }
  }
}
