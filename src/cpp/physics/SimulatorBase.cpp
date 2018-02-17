#include "SimulatorBase.h"

namespace alcube::physics {
  using namespace utils::opencl::kernelargs;
  namespace memories {
    opencl::dtos::Grid& Grid::at(int i) {
      return dto[i];
    }

    opencl::dtos::FluidSettings& FluidSettings::at(int i) {
      return dto[i];
    }

    opencl::dtos::FluidState& FluidState::at(int i) {
      return dto[i];
    }
  }

  void Kernels::inputFluid(
    unsigned int workSize,
    memories::FluidState &inputFluidStates,
    memories::FluidState &fluidStates
  ) {
    queue->push(rawKernels.inputFluid, {workSize}, {
      memArg(inputFluidStates.memory),
      memArg(fluidStates.memory)
    });
  }

  SimulatorBase::SimulatorBase(
    utils::opencl::ResourcesProvider *resourcesProvider,
    unsigned int maxActorCount,
    unsigned int maxActorCountForBitonicSort,
    unsigned int maxSpringCount,
    unsigned int allGridCount
  ) : utils::opencl::Simulator(resourcesProvider) {
    this->maxActorCount = maxActorCount;
    this->maxActorCountForBitonicSort = maxActorCountForBitonicSort;
    this->maxSpringCount = maxSpringCount;
    this->allGridCount = allGridCount;

    cl_program program = programFactory->create("../src/kernels/generated-code/all.cl");
    _kernels.fillGridIndex = kernelFactory->create(program, "fillGridIndex");
    _kernels.merge = kernelFactory->create(program, "merge");
    _kernels.bitonic = kernelFactory->create(program, "bitonic");
    _kernels.setGridRelationIndexRange = kernelFactory->create(program, "setGridRelationIndexRange");
    _kernels.initGridAndActorRelations = kernelFactory->create(program, "initGridAndActorRelations");
    _kernels.collectIntersections = kernelFactory->create(program, "collectIntersections");
    _kernels.updateByPenaltyImpulse = kernelFactory->create(program, "updateByPenaltyImpulse");
    _kernels.updateByFrictionalImpulse = kernelFactory->create(program, "updateByFrictionalImpulse");
    _kernels.collectCollisions = kernelFactory->create(program, "collectCollisions");
    _kernels.updateByConstraintImpulse = kernelFactory->create(program, "updateByConstraintImpulse");
    _kernels.calcSpringImpulses = kernelFactory->create(program, "calcSpringImpulses");
    _kernels.updateBySpringImpulse = kernelFactory->create(program, "updateBySpringImpulse");
    _kernels.postProcessing = kernelFactory->create(program, "postProcessing");
    _kernels.inputFluid = kernelFactory->create(program, "inputFluid");
    _kernels.updateDensityAndPressure = kernelFactory->create(program, "updateDensityAndPressure");
    _kernels.updateFluidForce = kernelFactory->create(program, "updateFluidForce");
    _kernels.moveFluid = kernelFactory->create(program, "moveFluid");
    _kernels.inputConstants = kernelFactory->create(program, "inputConstants");

    dtos.grid = new opencl::dtos::Grid();
    dtos.fluidSettings = new opencl::dtos::FluidSettings();
    dtos.actors = new opencl::dtos::Actor[maxActorCount];
    dtos.actorStates = new opencl::dtos::ActorState[maxActorCount];
    dtos.currentStates = new opencl::dtos::RigidBodyState[maxActorCount];
    dtos.nextStates = new opencl::dtos::RigidBodyState[maxActorCount];
    dtos.gridAndActorRelations = new opencl::dtos::GridAndActorRelation[maxActorCount];
    dtos.fluidStates = new opencl::dtos::FluidState[maxActorCount];
    dtos.inputFluidStates = new opencl::dtos::FluidState[maxActorCount];
    dtos.gridStartIndices = new unsigned int[allGridCount];
    dtos.gridEndIndices = new unsigned int[allGridCount];
    dtos.springs = new opencl::dtos::Spring[maxSpringCount];

    memories._grid             = defineHostMemory("grid", sizeof(opencl::dtos::Grid), dtos.grid, 1);
    memories._actors           = defineHostMemory("actors", sizeof(opencl::dtos::Actor), dtos.actors, maxActorCount);
    memories._currentStates    = defineHostMemory("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, maxActorCount);
    memories._springs          = defineHostMemory("springs", sizeof(opencl::dtos::Spring), dtos.springs, maxSpringCount);
    memories._inputFluidStates = defineHostMemory("inputFluidStates", sizeof(opencl::dtos::FluidState), dtos.inputFluidStates, maxActorCount);
    memories._fluidSettings    = defineHostMemory("fluidSettings", sizeof(opencl::dtos::FluidSettings), dtos.fluidSettings, 1);

    memories._actorStates           = defineGPUMemory("actorStates", sizeof(opencl::dtos::ActorState), maxActorCount);
    memories._nextStates            = defineGPUMemory("nextStates", sizeof(opencl::dtos::RigidBodyState), maxActorCount);
    memories._gridAndActorRelations = defineGPUMemory("gridAndActorRelations", sizeof(opencl::dtos::GridAndActorRelation), maxActorCountForBitonicSort);
    memories._gridStartIndices      = defineGPUMemory("gridStartIndices", sizeof(unsigned int), allGridCount);
    memories._gridEndIndices        = defineGPUMemory("gridEndIndices", sizeof(unsigned int), allGridCount);
    memories._springVars            = defineGPUMemory("springVars", sizeof(opencl::dtos::SpringVar), maxSpringCount);
    memories._fluidStates           = defineGPUMemory("fluidStates", sizeof(opencl::dtos::FluidState), maxActorCount);
    memories._constants             = defineGPUMemory("constants", sizeof(opencl::dtos::Constants), 1);

    memories.grid.dto = dtos.grid;
    memories.grid.memory = memories._grid;
    memories.inputFluidStates.dto = dtos.inputFluidStates;
    memories.inputFluidStates.memory = memories._inputFluidStates;
    memories.fluidSettings.dto = dtos.fluidSettings;
    memories.fluidSettings.memory = memories._fluidSettings;
    memories.fluidStates.dto = dtos.fluidStates;
    memories.fluidStates.memory = memories._fluidStates;

    memoryManager->allocate();
    kernels.rawKernels = _kernels;
    kernels.queue = queue;
  }

  utils::opencl::Memory* SimulatorBase::defineHostMemory(
    const std::string &name,
    size_t size,
    void *hostPtr,
    size_t allocationCount
  ) {
    return memoryManager->define(name, size, hostPtr, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, allocationCount);
  }

  utils::opencl::Memory* SimulatorBase::defineGPUMemory(
    const std::string &name,
    size_t size,
    size_t allocationCount
  ) {
    return memoryManager->define(name, size, nullptr, CL_MEM_READ_WRITE, allocationCount);
  }
}