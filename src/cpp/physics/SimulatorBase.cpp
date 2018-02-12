#include "SimulatorBase.h"

namespace alcube::physics {
  SimulatorBase::SimulatorBase(
    utils::opencl::Resources *resources,
    utils::FileUtil *fileUtil,
    unsigned int maxActorCount,
    unsigned int maxActorCountForBitonicSort,
    unsigned int maxSpringCount,
    unsigned int allGridCount
  ) : utils::opencl::Simulator(resources, fileUtil) {
    this->maxActorCount = maxActorCount;
    this->maxActorCountForBitonicSort = maxActorCountForBitonicSort;
    this->maxSpringCount = maxSpringCount;
    this->allGridCount = allGridCount;

    cl_program program = programFactory->create({
        "../src/kernels/physics/dtos.cl",
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
    kernels.initGridAndActorRelations = kernelFactory->create(program, "initGridAndActorRelations");
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

    memories.grid = memoryManager->define("grid", sizeof(opencl::dtos::Grid), dtos.grid, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 1, 1);
    memories.actors = memoryManager->define("actors", sizeof(opencl::dtos::Actor), dtos.actors, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxActorCount, 0);
    memories.currentStates = memoryManager->define("currentStates", sizeof(opencl::dtos::RigidBodyState), dtos.currentStates, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxActorCount, 0);
    memories.actorStates = memoryManager->define("actorStates", sizeof(opencl::dtos::ActorState), nullptr, CL_MEM_READ_WRITE, maxActorCount, 0);
    memories.nextStates = memoryManager->define("nextStates", sizeof(opencl::dtos::RigidBodyState), nullptr, CL_MEM_READ_WRITE, maxActorCount, 0);
    memories.gridAndActorRelations = memoryManager->define("gridAndActorRelations", sizeof(opencl::dtos::GridAndActorRelation), nullptr, CL_MEM_READ_WRITE, maxActorCountForBitonicSort, 0);
    memories.gridStartIndices = memoryManager->define("gridStartIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE, allGridCount, allGridCount);
    memories.gridEndIndices = memoryManager->define("gridEndIndices", sizeof(unsigned int), nullptr, CL_MEM_READ_WRITE, allGridCount, allGridCount);
    memories.springs = memoryManager->define("springs", sizeof(opencl::dtos::Spring), dtos.springs, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxSpringCount, 0);
    memories.springVars = memoryManager->define("springVars", sizeof(opencl::dtos::SpringVar), nullptr, CL_MEM_READ_WRITE, maxSpringCount, 0);
    memories.inputFluidStates = memoryManager->define("inputFluidStates", sizeof(opencl::dtos::FluidState), dtos.inputFluidStates, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, maxActorCount, 0);
    memories.fluidStates = memoryManager->define("fluidStates", sizeof(opencl::dtos::FluidState), nullptr, CL_MEM_READ_WRITE, maxActorCount, 0);
    memories.fluidSettings = memoryManager->define("fluidSettings", sizeof(opencl::dtos::FluidSettings), dtos.fluidSettings, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 1, 1);
    memories.constants = memoryManager->define("constants", sizeof(opencl::dtos::Constants), nullptr, CL_MEM_READ_WRITE, 1, 1);

    memoryManager->allocate();
  }
}