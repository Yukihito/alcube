#include "GPU.h"

namespace alcube::gpu {
  using namespace utils::opencl::kernelargs;
  namespace memories {
    dtos::Actor* Actor::at(int i) {
      return &dto[i];
    }

    void Actor::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Actor::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::ActorState* ActorState::at(int i) {
      return &dto[i];
    }

    void ActorState::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void ActorState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::Constants* Constants::at(int i) {
      return &dto[i];
    }

    void Constants::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Constants::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::FluidSettings* FluidSettings::at(int i) {
      return &dto[i];
    }

    void FluidSettings::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void FluidSettings::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::FluidState* FluidState::at(int i) {
      return &dto[i];
    }

    void FluidState::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void FluidState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::Grid* Grid::at(int i) {
      return &dto[i];
    }

    void Grid::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Grid::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::GridAndActorRelation* GridAndActorRelation::at(int i) {
      return &dto[i];
    }

    void GridAndActorRelation::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void GridAndActorRelation::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::Intersection* Intersection::at(int i) {
      return &dto[i];
    }

    void Intersection::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Intersection::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::RigidBodyState* RigidBodyState::at(int i) {
      return &dto[i];
    }

    void RigidBodyState::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void RigidBodyState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::Spring* Spring::at(int i) {
      return &dto[i];
    }

    void Spring::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Spring::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    dtos::SpringVar* SpringVar::at(int i) {
      return &dto[i];
    }

    void SpringVar::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void SpringVar::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    char* CharMemory::at(int i) {
      return &dto[i];
    }

    void CharMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void CharMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    double* DoubleMemory::at(int i) {
      return &dto[i];
    }

    void DoubleMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void DoubleMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    float* FloatMemory::at(int i) {
      return &dto[i];
    }

    void FloatMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void FloatMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    cl_float3* Float3Memory::at(int i) {
      return &dto[i];
    }

    void Float3Memory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Float3Memory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    cl_float4* Float4Memory::at(int i) {
      return &dto[i];
    }

    void Float4Memory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Float4Memory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    int* IntMemory::at(int i) {
      return &dto[i];
    }

    void IntMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void IntMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    long* LongMemory::at(int i) {
      return &dto[i];
    }

    void LongMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void LongMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    short* ShortMemory::at(int i) {
      return &dto[i];
    }

    void ShortMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void ShortMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    unsigned char* UcharMemory::at(int i) {
      return &dto[i];
    }

    void UcharMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void UcharMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    unsigned int* UintMemory::at(int i) {
      return &dto[i];
    }

    void UintMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void UintMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    unsigned long* UlongMemory::at(int i) {
      return &dto[i];
    }

    void UlongMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void UlongMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    unsigned short* UshortMemory::at(int i) {
      return &dto[i];
    }

    void UshortMemory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void UshortMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }
  }

  void Kernels::inputConstants(
    unsigned int workSize,
    memories::Constants& constants,
    memories::Grid& grid,
    memories::FluidSettings& fluidSettings,
    float gravityAcceleration,
    float deltaTime,
    float splitDeltaTime,
    float sphericalShellRadius,
    unsigned short rigidBodyParticleCount,
    unsigned short fluidParticleCount
  ) {
    queue->push(rawKernels.inputConstants, {workSize}, {
      memArg(constants.memory),
      memArg(grid.memory),
      memArg(fluidSettings.memory),
      floatArg(gravityAcceleration),
      floatArg(deltaTime),
      floatArg(splitDeltaTime),
      floatArg(sphericalShellRadius),
      ushortArg(rigidBodyParticleCount),
      ushortArg(fluidParticleCount)
    });
  }

  void Kernels::initGridAndActorRelations(
    unsigned int workSize,
    memories::GridAndActorRelation& relations,
    unsigned int gridIndex,
    unsigned short actorIndex
  ) {
    queue->push(rawKernels.initGridAndActorRelations, {workSize}, {
      memArg(relations.memory),
      uintArg(gridIndex),
      ushortArg(actorIndex)
    });
  }

  void Kernels::fillGridIndex(
    unsigned int workSize,
    memories::Grid& grid,
    memories::Actor& actors,
    memories::ActorState& actorStates,
    memories::RigidBodyState& currentStates,
    memories::RigidBodyState& nextStates,
    memories::GridAndActorRelation& relations
  ) {
    queue->push(rawKernels.fillGridIndex, {workSize}, {
      memArg(grid.memory),
      memArg(actors.memory),
      memArg(actorStates.memory),
      memArg(currentStates.memory),
      memArg(nextStates.memory),
      memArg(relations.memory)
    });
  }

  void Kernels::merge(
    unsigned int workSize,
    memories::GridAndActorRelation& relations,
    unsigned int distance
  ) {
    queue->push(rawKernels.merge, {workSize}, {
      memArg(relations.memory),
      uintArg(distance)
    });
  }

  void Kernels::bitonic(
    unsigned int workSize,
    memories::GridAndActorRelation& relations,
    unsigned int distance,
    unsigned int stageDistance
  ) {
    queue->push(rawKernels.bitonic, {workSize}, {
      memArg(relations.memory),
      uintArg(distance),
      uintArg(stageDistance)
    });
  }

  void Kernels::setGridRelationIndexRange(
    unsigned int workSize,
    memories::GridAndActorRelation& relations,
    memories::UintMemory& gridStartIndices,
    memories::UintMemory& gridEndIndices,
    unsigned int actorCount
  ) {
    queue->push(rawKernels.setGridRelationIndexRange, {workSize}, {
      memArg(relations.memory),
      memArg(gridStartIndices.memory),
      memArg(gridEndIndices.memory),
      uintArg(actorCount)
    });
  }

  void Kernels::updateByPenaltyImpulse(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates,
    float deltaTime
  ) {
    queue->push(rawKernels.updateByPenaltyImpulse, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory),
      floatArg(deltaTime)
    });
  }

  void Kernels::updateByFrictionalImpulse(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates
  ) {
    queue->push(rawKernels.updateByFrictionalImpulse, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory)
    });
  }

  void Kernels::collectCollisions(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates
  ) {
    queue->push(rawKernels.collectCollisions, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory)
    });
  }

  void Kernels::updateByConstraintImpulse(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates
  ) {
    queue->push(rawKernels.updateByConstraintImpulse, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory)
    });
  }

  void Kernels::inputFluid(
    unsigned int workSize,
    memories::FluidState& inputFluidStates,
    memories::FluidState& fluidStates
  ) {
    queue->push(rawKernels.inputFluid, {workSize}, {
      memArg(inputFluidStates.memory),
      memArg(fluidStates.memory)
    });
  }

  void Kernels::updateDensityAndPressure(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::FluidState& fluidStates,
    memories::Constants& constants
  ) {
    queue->push(rawKernels.updateDensityAndPressure, {workSize}, {
      memArg(actorStates.memory),
      memArg(fluidStates.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::updateFluidForce(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::FluidState& fluidStates,
    memories::Constants& constants
  ) {
    queue->push(rawKernels.updateFluidForce, {workSize}, {
      memArg(actorStates.memory),
      memArg(fluidStates.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::moveFluid(
    unsigned int workSize,
    memories::FluidState& fluidStates,
    memories::RigidBodyState& nextStates,
    memories::Constants& constants
  ) {
    queue->push(rawKernels.moveFluid, {workSize}, {
      memArg(fluidStates.memory),
      memArg(nextStates.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::postProcessing(
    unsigned int workSize,
    memories::Grid& grid,
    memories::Actor& actors,
    memories::ActorState& actorStates,
    memories::RigidBodyState& nextStates,
    float deltaTime
  ) {
    queue->push(rawKernels.postProcessing, {workSize}, {
      memArg(grid.memory),
      memArg(actors.memory),
      memArg(actorStates.memory),
      memArg(nextStates.memory),
      floatArg(deltaTime)
    });
  }

  void Kernels::collectIntersections(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::Spring& springs,
    memories::RigidBodyState& nextStates,
    memories::GridAndActorRelation& relations,
    memories::UintMemory& gridStartIndices,
    memories::UintMemory& gridEndIndices,
    memories::Constants& constants
  ) {
    queue->push(rawKernels.collectIntersections, {workSize}, {
      memArg(actorStates.memory),
      memArg(springs.memory),
      memArg(nextStates.memory),
      memArg(relations.memory),
      memArg(gridStartIndices.memory),
      memArg(gridEndIndices.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::calcSpringImpulses(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::Spring& springs,
    memories::SpringVar& springVars,
    memories::RigidBodyState& nextStates,
    float deltaTime
  ) {
    queue->push(rawKernels.calcSpringImpulses, {workSize}, {
      memArg(actorStates.memory),
      memArg(springs.memory),
      memArg(springVars.memory),
      memArg(nextStates.memory),
      floatArg(deltaTime)
    });
  }

  void Kernels::updateBySpringImpulse(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates,
    memories::RigidBodyState& nextStates,
    memories::SpringVar& springVars,
    float deltaTime
  ) {
    queue->push(rawKernels.updateBySpringImpulse, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory),
      memArg(nextStates.memory),
      memArg(springVars.memory),
      floatArg(deltaTime)
    });
  }

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

    kernels.rawKernels.inputConstants = resourcesProvider->kernelFactory->create(program, "inputConstants");
    kernels.rawKernels.initGridAndActorRelations = resourcesProvider->kernelFactory->create(program, "initGridAndActorRelations");
    kernels.rawKernels.fillGridIndex = resourcesProvider->kernelFactory->create(program, "fillGridIndex");
    kernels.rawKernels.merge = resourcesProvider->kernelFactory->create(program, "merge");
    kernels.rawKernels.bitonic = resourcesProvider->kernelFactory->create(program, "bitonic");
    kernels.rawKernels.setGridRelationIndexRange = resourcesProvider->kernelFactory->create(program, "setGridRelationIndexRange");
    kernels.rawKernels.updateByPenaltyImpulse = resourcesProvider->kernelFactory->create(program, "updateByPenaltyImpulse");
    kernels.rawKernels.updateByFrictionalImpulse = resourcesProvider->kernelFactory->create(program, "updateByFrictionalImpulse");
    kernels.rawKernels.collectCollisions = resourcesProvider->kernelFactory->create(program, "collectCollisions");
    kernels.rawKernels.updateByConstraintImpulse = resourcesProvider->kernelFactory->create(program, "updateByConstraintImpulse");
    kernels.rawKernels.inputFluid = resourcesProvider->kernelFactory->create(program, "inputFluid");
    kernels.rawKernels.updateDensityAndPressure = resourcesProvider->kernelFactory->create(program, "updateDensityAndPressure");
    kernels.rawKernels.updateFluidForce = resourcesProvider->kernelFactory->create(program, "updateFluidForce");
    kernels.rawKernels.moveFluid = resourcesProvider->kernelFactory->create(program, "moveFluid");
    kernels.rawKernels.postProcessing = resourcesProvider->kernelFactory->create(program, "postProcessing");
    kernels.rawKernels.collectIntersections = resourcesProvider->kernelFactory->create(program, "collectIntersections");
    kernels.rawKernels.calcSpringImpulses = resourcesProvider->kernelFactory->create(program, "calcSpringImpulses");
    kernels.rawKernels.updateBySpringImpulse = resourcesProvider->kernelFactory->create(program, "updateBySpringImpulse");

    dtos.grid = new dtos::Grid();
    dtos.actors = new dtos::Actor[maxActorCount];
    dtos.currentStates = new dtos::RigidBodyState[maxActorCount];
    dtos.springs = new dtos::Spring[maxSpringCount];
    dtos.inputFluidStates = new dtos::FluidState[maxActorCount];
    dtos.fluidSettings = new dtos::FluidSettings();
    dtos.actorStates = new dtos::ActorState[maxActorCount];
    dtos.nextStates = new dtos::RigidBodyState[maxActorCount];
    dtos.gridAndActorRelations = new dtos::GridAndActorRelation[maxActorCountForBitonicSort];
    dtos.gridStartIndices = new unsigned int[allGridCount];
    dtos.gridEndIndices = new unsigned int[allGridCount];
    dtos.springVars = new dtos::SpringVar[maxSpringCount];
    dtos.fluidStates = new dtos::FluidState[maxActorCount];
    dtos.constants = new dtos::Constants();

    memories.grid.memory = defineHostMemory("grid", sizeof(dtos::Grid), dtos.grid, 1);
    memories.actors.memory = defineHostMemory("actors", sizeof(dtos::Actor), dtos.actors, maxActorCount);
    memories.currentStates.memory = defineHostMemory("currentStates", sizeof(dtos::RigidBodyState), dtos.currentStates, maxActorCount);
    memories.springs.memory = defineHostMemory("springs", sizeof(dtos::Spring), dtos.springs, maxSpringCount);
    memories.inputFluidStates.memory = defineHostMemory("inputFluidStates", sizeof(dtos::FluidState), dtos.inputFluidStates, maxActorCount);
    memories.fluidSettings.memory = defineHostMemory("fluidSettings", sizeof(dtos::FluidSettings), dtos.fluidSettings, 1);
    memories.actorStates.memory = defineGPUMemory("actorStates", sizeof(dtos::ActorState), maxActorCount);
    memories.nextStates.memory = defineGPUMemory("nextStates", sizeof(dtos::RigidBodyState), maxActorCount);
    memories.gridAndActorRelations.memory = defineGPUMemory("gridAndActorRelations", sizeof(dtos::GridAndActorRelation), maxActorCountForBitonicSort);
    memories.gridStartIndices.memory = defineGPUMemory("gridStartIndices", sizeof(unsigned int), allGridCount);
    memories.gridEndIndices.memory = defineGPUMemory("gridEndIndices", sizeof(unsigned int), allGridCount);
    memories.springVars.memory = defineGPUMemory("springVars", sizeof(dtos::SpringVar), maxSpringCount);
    memories.fluidStates.memory = defineGPUMemory("fluidStates", sizeof(dtos::FluidState), maxActorCount);
    memories.constants.memory = defineGPUMemory("constants", sizeof(dtos::Constants), 1);

    memories.grid.dto = dtos.grid;
    memories.actors.dto = dtos.actors;
    memories.currentStates.dto = dtos.currentStates;
    memories.springs.dto = dtos.springs;
    memories.inputFluidStates.dto = dtos.inputFluidStates;
    memories.fluidSettings.dto = dtos.fluidSettings;
    memories.actorStates.dto = dtos.actorStates;
    memories.nextStates.dto = dtos.nextStates;
    memories.gridAndActorRelations.dto = dtos.gridAndActorRelations;
    memories.gridStartIndices.dto = dtos.gridStartIndices;
    memories.gridEndIndices.dto = dtos.gridEndIndices;
    memories.springVars.dto = dtos.springVars;
    memories.fluidStates.dto = dtos.fluidStates;
    memories.constants.dto = dtos.constants;

    memories.grid.resourcesProvider = resourcesProvider;
    memories.actors.resourcesProvider = resourcesProvider;
    memories.currentStates.resourcesProvider = resourcesProvider;
    memories.springs.resourcesProvider = resourcesProvider;
    memories.inputFluidStates.resourcesProvider = resourcesProvider;
    memories.fluidSettings.resourcesProvider = resourcesProvider;
    memories.actorStates.resourcesProvider = resourcesProvider;
    memories.nextStates.resourcesProvider = resourcesProvider;
    memories.gridAndActorRelations.resourcesProvider = resourcesProvider;
    memories.gridStartIndices.resourcesProvider = resourcesProvider;
    memories.gridEndIndices.resourcesProvider = resourcesProvider;
    memories.springVars.resourcesProvider = resourcesProvider;
    memories.fluidStates.resourcesProvider = resourcesProvider;
    memories.constants.resourcesProvider = resourcesProvider;;

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
