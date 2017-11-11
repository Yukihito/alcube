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
    kernelFactory = new utils::opencl::KernelFactory(resources, fileUtil);
    programFactory = new utils::opencl::ProgramFactory(resources, fileUtil);
    memoryManager = new utils::opencl::MemoryManager(resources);
    queue = new utils::opencl::CommandQueue(resources);
    cells = {};

    cl_program setupGridProgram = programFactory->create("../src/kernels/physics/setupgrid.cl");
    kernels.fillGridIndex = kernelFactory->create(setupGridProgram, "fillGridIndex");

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

    memories.grid = memoryManager->define("grid", sizeof(opencl::dtos::Grid), dtos.grid, true, false);
    memories.cells = memoryManager->define("cells", sizeof(opencl::dtos::Cell), dtos.cells, true, false);
    memories.currentStates = memoryManager->define("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, true, true);
    memories.gridAndCellRelations = memoryManager->define("gridAndCellRelations", sizeof(opencl::dtos::GridAndCellRelation), dtos.gridAndCellRelations, true, true);
  }

  void Simulator::update(float deltaTime) {
    unsigned long cellsCount = cells.size();

    // Model -> DTO
    for (int i = 0; i < cellsCount; i++) {
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

    memories.cells->count = cellsCount;
    memories.currentStates->count = cellsCount;
    memories.gridAndCellRelations->count = cellsCount;
    memoryManager->allocate();

    // Computing start
    queue->push(kernels.fillGridIndex, {cellsCount}, {
      memArg(memories.grid),
      memArg(memories.cells),
      memArg(memories.currentStates),
      memArg(memories.gridAndCellRelations)
    });
    queue->read(memories.currentStates, dtos.currentStates);
    queue->read(memories.gridAndCellRelations, dtos.gridAndCellRelations);
    // Computing finish

    memoryManager->release();

    // DTO -> Model
    for (int i = 0; i < cellsCount; i++) {
      Cell* cell = cells[i];
      assignGlmVec3(cell->currentState.linearMomentum, dtos.currentStates[i].linearMomentum);
      assignGlmVec3(cell->currentState.angularMomentum, dtos.currentStates[i].angularMomentum);
      assignGlmVec3(cell->currentState.position, dtos.currentStates[i].position);
      assignGlmQuat(cell->currentState.rotation, dtos.currentStates[i].rotation);
    }
  }
}
