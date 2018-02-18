#include "GPU.h"

namespace alcube::gpu {
  using namespace utils::opencl::kernelargs;
  // Memories
  namespace memories {
    dtos::Grid& Grid::at(int i) {
      return dto[i];
    }

    dtos::FluidSettings& FluidSettings::at(int i) {
      return dto[i];
    }

    dtos::FluidState& FluidState::at(int i) {
      return dto[i];
    }
  }

  // Kernels
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

  // GPU
  GPU::GPU(
    utils::opencl::ResourcesProvider *resourcesProvider,
    unsigned int maxActorCount,
    unsigned int maxActorCountForBitonicSort,
    unsigned int maxSpringCount,
    unsigned int allGridCount
  ) {
    this->resourcesProvider = resourcesProvider;
    cl_program program = resourcesProvider->programFactory->create("../src/kernels/generated-code/all.cl");

    kernels.queue = resourcesProvider->queue;
    kernels.rawKernels.fillGridIndex             = resourcesProvider->kernelFactory->create(program, "fillGridIndex");
    kernels.rawKernels.merge                     = resourcesProvider->kernelFactory->create(program, "merge");
    kernels.rawKernels.bitonic                   = resourcesProvider->kernelFactory->create(program, "bitonic");
    kernels.rawKernels.setGridRelationIndexRange = resourcesProvider->kernelFactory->create(program, "setGridRelationIndexRange");
    kernels.rawKernels.initGridAndActorRelations = resourcesProvider->kernelFactory->create(program, "initGridAndActorRelations");
    kernels.rawKernels.collectIntersections      = resourcesProvider->kernelFactory->create(program, "collectIntersections");
    kernels.rawKernels.updateByPenaltyImpulse    = resourcesProvider->kernelFactory->create(program, "updateByPenaltyImpulse");
    kernels.rawKernels.updateByFrictionalImpulse = resourcesProvider->kernelFactory->create(program, "updateByFrictionalImpulse");
    kernels.rawKernels.collectCollisions         = resourcesProvider->kernelFactory->create(program, "collectCollisions");
    kernels.rawKernels.updateByConstraintImpulse = resourcesProvider->kernelFactory->create(program, "updateByConstraintImpulse");
    kernels.rawKernels.calcSpringImpulses        = resourcesProvider->kernelFactory->create(program, "calcSpringImpulses");
    kernels.rawKernels.updateBySpringImpulse     = resourcesProvider->kernelFactory->create(program, "updateBySpringImpulse");
    kernels.rawKernels.postProcessing            = resourcesProvider->kernelFactory->create(program, "postProcessing");
    kernels.rawKernels.inputFluid                = resourcesProvider->kernelFactory->create(program, "inputFluid");
    kernels.rawKernels.updateDensityAndPressure  = resourcesProvider->kernelFactory->create(program, "updateDensityAndPressure");
    kernels.rawKernels.updateFluidForce          = resourcesProvider->kernelFactory->create(program, "updateFluidForce");
    kernels.rawKernels.moveFluid                 = resourcesProvider->kernelFactory->create(program, "moveFluid");
    kernels.rawKernels.inputConstants            = resourcesProvider->kernelFactory->create(program, "inputConstants");

    dtos.grid                  = new dtos::Grid();
    dtos.fluidSettings         = new dtos::FluidSettings();
    dtos.actors                = new dtos::Actor[maxActorCount];
    dtos.actorStates           = new dtos::ActorState[maxActorCount];
    dtos.currentStates         = new dtos::RigidBodyState[maxActorCount];
    dtos.nextStates            = new dtos::RigidBodyState[maxActorCount];
    dtos.gridAndActorRelations = new dtos::GridAndActorRelation[maxActorCount];
    dtos.fluidStates           = new dtos::FluidState[maxActorCount];
    dtos.inputFluidStates      = new dtos::FluidState[maxActorCount];
    dtos.gridStartIndices      = new unsigned int[allGridCount];
    dtos.gridEndIndices        = new unsigned int[allGridCount];
    dtos.springs               = new dtos::Spring[maxSpringCount];

    memories.grid.memory             = defineHostMemory("grid", sizeof(dtos::Grid), dtos.grid, 1);
    memories.inputFluidStates.memory = defineHostMemory("inputFluidStates", sizeof(dtos::FluidState), dtos.inputFluidStates, maxActorCount);
    memories.fluidSettings.memory    = defineHostMemory("fluidSettings", sizeof(dtos::FluidSettings), dtos.fluidSettings, 1);
    memories.fluidStates.memory      = defineGPUMemory("fluidStates", sizeof(dtos::FluidState), maxActorCount);

    memories.grid.dto             = dtos.grid;
    memories.inputFluidStates.dto = dtos.inputFluidStates;
    memories.fluidSettings.dto    = dtos.fluidSettings;
    memories.fluidStates.dto      = dtos.fluidStates;

    resourcesProvider->memoryManager->allocate();
  }

  utils::opencl::Memory* GPU::defineHostMemory(
    const std::string &name,
    size_t size,
    void *hostPtr,
    size_t allocationCount
  ) {
    return resourcesProvider->memoryManager->define(name, size, hostPtr, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, allocationCount);
  }

  utils::opencl::Memory* GPU::defineGPUMemory(
    const std::string &name,
    size_t size,
    size_t allocationCount
  ) {
    return resourcesProvider->memoryManager->define(name, size, nullptr, CL_MEM_READ_WRITE, allocationCount);
  }
}