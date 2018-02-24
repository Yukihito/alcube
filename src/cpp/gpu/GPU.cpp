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

    void Actor::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Actor::setCount(size_t count) {
      this->memory->count = count;
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

    void ActorState::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void ActorState::setCount(size_t count) {
      this->memory->count = count;
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

    void Constants::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Constants::setCount(size_t count) {
      this->memory->count = count;
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

    void FluidSettings::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void FluidSettings::setCount(size_t count) {
      this->memory->count = count;
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

    void FluidState::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void FluidState::setCount(size_t count) {
      this->memory->count = count;
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

    void Grid::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Grid::setCount(size_t count) {
      this->memory->count = count;
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

    void GridAndActorRelation::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void GridAndActorRelation::setCount(size_t count) {
      this->memory->count = count;
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

    void Intersection::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Intersection::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::PhysicalQuantity* PhysicalQuantity::at(int i) {
      return &dto[i];
    }

    void PhysicalQuantity::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void PhysicalQuantity::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void PhysicalQuantity::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void PhysicalQuantity::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::SoftBodyState* SoftBodyState::at(int i) {
      return &dto[i];
    }

    void SoftBodyState::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void SoftBodyState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void SoftBodyState::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void SoftBodyState::setCount(size_t count) {
      this->memory->count = count;
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

    void Spring::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Spring::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::SpringState* SpringState::at(int i) {
      return &dto[i];
    }

    void SpringState::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void SpringState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void SpringState::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void SpringState::setCount(size_t count) {
      this->memory->count = count;
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

    void CharMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void CharMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void DoubleMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void DoubleMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void FloatMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void FloatMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void Float3Memory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Float3Memory::setCount(size_t count) {
      this->memory->count = count;
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

    void Float4Memory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void Float4Memory::setCount(size_t count) {
      this->memory->count = count;
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

    void IntMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void IntMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void LongMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void LongMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void ShortMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void ShortMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void UcharMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void UcharMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void UintMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void UintMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void UlongMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void UlongMemory::setCount(size_t count) {
      this->memory->count = count;
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

    void UshortMemory::read() {
      resourcesProvider->queue->read(this->memory, this->dto);
    }

    void UshortMemory::setCount(size_t count) {
      this->memory->count = count;
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
    float sphericalShellRadius
  ) {
    queue->push(rawKernels.inputConstants, {workSize}, {
      memArg(constants.memory),
      memArg(grid.memory),
      memArg(fluidSettings.memory),
      floatArg(gravityAcceleration),
      floatArg(deltaTime),
      floatArg(splitDeltaTime),
      floatArg(sphericalShellRadius)
    });
  }

  void Kernels::inputActors(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates,
    memories::PhysicalQuantity& hostPhysicalQuantities,
    memories::PhysicalQuantity& physicalQuantities
  ) {
    queue->push(rawKernels.inputActors, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory),
      memArg(hostPhysicalQuantities.memory),
      memArg(physicalQuantities.memory)
    });
  }

  void Kernels::inputSoftBodyStates(
    unsigned int workSize,
    memories::SoftBodyState& hostSoftBodyStates,
    memories::SoftBodyState& softBodyStates
  ) {
    queue->push(rawKernels.inputSoftBodyStates, {workSize}, {
      memArg(hostSoftBodyStates.memory),
      memArg(softBodyStates.memory)
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
    memories::Constants& constants,
    memories::PhysicalQuantity& physicalQuantities,
    memories::GridAndActorRelation& relations
  ) {
    queue->push(rawKernels.fillGridIndex, {workSize}, {
      memArg(constants.memory),
      memArg(physicalQuantities.memory),
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
    memories::ActorState& actorStates,
    memories::SoftBodyState& softBodyStates
  ) {
    queue->push(rawKernels.updateByConstraintImpulse, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory),
      memArg(softBodyStates.memory)
    });
  }

  void Kernels::inputFluid(
    unsigned int workSize,
    memories::FluidState& hostFluidStates,
    memories::FluidState& fluidStates
  ) {
    queue->push(rawKernels.inputFluid, {workSize}, {
      memArg(hostFluidStates.memory),
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
    memories::PhysicalQuantity& physicalQuantities,
    memories::Constants& constants
  ) {
    queue->push(rawKernels.moveFluid, {workSize}, {
      memArg(fluidStates.memory),
      memArg(physicalQuantities.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::postProcessing(
    unsigned int workSize,
    memories::Constants& constants,
    memories::ActorState& actorStates,
    memories::PhysicalQuantity& physicalQuantities,
    float deltaTime
  ) {
    queue->push(rawKernels.postProcessing, {workSize}, {
      memArg(constants.memory),
      memArg(actorStates.memory),
      memArg(physicalQuantities.memory),
      floatArg(deltaTime)
    });
  }

  void Kernels::collectIntersections(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::PhysicalQuantity& physicalQuantities,
    memories::GridAndActorRelation& relations,
    memories::UintMemory& gridStartIndices,
    memories::UintMemory& gridEndIndices,
    memories::Constants& constants
  ) {
    queue->push(rawKernels.collectIntersections, {workSize}, {
      memArg(actorStates.memory),
      memArg(physicalQuantities.memory),
      memArg(relations.memory),
      memArg(gridStartIndices.memory),
      memArg(gridEndIndices.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::calcSpringImpulses(
    unsigned int workSize,
    memories::Spring& springs,
    memories::SpringState& springStates,
    memories::PhysicalQuantity& physicalQuantities,
    float deltaTime
  ) {
    queue->push(rawKernels.calcSpringImpulses, {workSize}, {
      memArg(springs.memory),
      memArg(springStates.memory),
      memArg(physicalQuantities.memory),
      floatArg(deltaTime)
    });
  }

  void Kernels::updateBySpringImpulse(
    unsigned int workSize,
    memories::SoftBodyState& softBodyStates,
    memories::ActorState& actorStates,
    memories::PhysicalQuantity& physicalQuantities,
    memories::SpringState& springStates,
    float deltaTime
  ) {
    queue->push(rawKernels.updateBySpringImpulse, {workSize}, {
      memArg(softBodyStates.memory),
      memArg(actorStates.memory),
      memArg(physicalQuantities.memory),
      memArg(springStates.memory),
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
    kernels.rawKernels.inputActors = resourcesProvider->kernelFactory->create(program, "inputActors");
    kernels.rawKernels.inputSoftBodyStates = resourcesProvider->kernelFactory->create(program, "inputSoftBodyStates");
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
    dtos.hostPhysicalQuantities = new dtos::PhysicalQuantity[maxActorCount];
    dtos.springs = new dtos::Spring[maxSpringCount];
    dtos.hostFluidStates = new dtos::FluidState[maxActorCount];
    dtos.fluidSettings = new dtos::FluidSettings();
    dtos.actorStates = new dtos::ActorState[maxActorCount];
    dtos.physicalQuantities = new dtos::PhysicalQuantity[maxActorCount];
    dtos.gridAndActorRelations = new dtos::GridAndActorRelation[maxActorCountForBitonicSort];
    dtos.gridStartIndices = new unsigned int[allGridCount];
    dtos.gridEndIndices = new unsigned int[allGridCount];
    dtos.springStates = new dtos::SpringState[maxSpringCount];
    dtos.fluidStates = new dtos::FluidState[maxActorCount];
    dtos.constants = new dtos::Constants();
    dtos.hostSoftBodyStates = new dtos::SoftBodyState[maxActorCount];
    dtos.softBodyStates = new dtos::SoftBodyState[maxActorCount];

    memories.grid.memory = defineHostMemory("grid", sizeof(dtos::Grid), dtos.grid, 1);
    memories.actors.memory = defineHostMemory("actors", sizeof(dtos::Actor), dtos.actors, maxActorCount);
    memories.hostPhysicalQuantities.memory = defineHostMemory("hostPhysicalQuantities", sizeof(dtos::PhysicalQuantity), dtos.hostPhysicalQuantities, maxActorCount);
    memories.springs.memory = defineHostMemory("springs", sizeof(dtos::Spring), dtos.springs, maxSpringCount);
    memories.hostFluidStates.memory = defineHostMemory("hostFluidStates", sizeof(dtos::FluidState), dtos.hostFluidStates, maxActorCount);
    memories.fluidSettings.memory = defineHostMemory("fluidSettings", sizeof(dtos::FluidSettings), dtos.fluidSettings, 1);
    memories.actorStates.memory = defineGPUMemory("actorStates", sizeof(dtos::ActorState), maxActorCount);
    memories.physicalQuantities.memory = defineGPUMemory("physicalQuantities", sizeof(dtos::PhysicalQuantity), maxActorCount);
    memories.gridAndActorRelations.memory = defineGPUMemory("gridAndActorRelations", sizeof(dtos::GridAndActorRelation), maxActorCountForBitonicSort);
    memories.gridStartIndices.memory = defineGPUMemory("gridStartIndices", sizeof(unsigned int), allGridCount);
    memories.gridEndIndices.memory = defineGPUMemory("gridEndIndices", sizeof(unsigned int), allGridCount);
    memories.springStates.memory = defineGPUMemory("springStates", sizeof(dtos::SpringState), maxSpringCount);
    memories.fluidStates.memory = defineGPUMemory("fluidStates", sizeof(dtos::FluidState), maxActorCount);
    memories.constants.memory = defineGPUMemory("constants", sizeof(dtos::Constants), 1);
    memories.hostSoftBodyStates.memory = defineHostMemory("hostSoftBodyStates", sizeof(dtos::SoftBodyState), dtos.hostSoftBodyStates, maxActorCount);
    memories.softBodyStates.memory = defineGPUMemory("softBodyStates", sizeof(dtos::SoftBodyState), maxActorCount);

    memories.grid.dto = dtos.grid;
    memories.actors.dto = dtos.actors;
    memories.hostPhysicalQuantities.dto = dtos.hostPhysicalQuantities;
    memories.springs.dto = dtos.springs;
    memories.hostFluidStates.dto = dtos.hostFluidStates;
    memories.fluidSettings.dto = dtos.fluidSettings;
    memories.actorStates.dto = dtos.actorStates;
    memories.physicalQuantities.dto = dtos.physicalQuantities;
    memories.gridAndActorRelations.dto = dtos.gridAndActorRelations;
    memories.gridStartIndices.dto = dtos.gridStartIndices;
    memories.gridEndIndices.dto = dtos.gridEndIndices;
    memories.springStates.dto = dtos.springStates;
    memories.fluidStates.dto = dtos.fluidStates;
    memories.constants.dto = dtos.constants;
    memories.hostSoftBodyStates.dto = dtos.hostSoftBodyStates;
    memories.softBodyStates.dto = dtos.softBodyStates;

    memories.grid.resourcesProvider = resourcesProvider;
    memories.actors.resourcesProvider = resourcesProvider;
    memories.hostPhysicalQuantities.resourcesProvider = resourcesProvider;
    memories.springs.resourcesProvider = resourcesProvider;
    memories.hostFluidStates.resourcesProvider = resourcesProvider;
    memories.fluidSettings.resourcesProvider = resourcesProvider;
    memories.actorStates.resourcesProvider = resourcesProvider;
    memories.physicalQuantities.resourcesProvider = resourcesProvider;
    memories.gridAndActorRelations.resourcesProvider = resourcesProvider;
    memories.gridStartIndices.resourcesProvider = resourcesProvider;
    memories.gridEndIndices.resourcesProvider = resourcesProvider;
    memories.springStates.resourcesProvider = resourcesProvider;
    memories.fluidStates.resourcesProvider = resourcesProvider;
    memories.constants.resourcesProvider = resourcesProvider;
    memories.hostSoftBodyStates.resourcesProvider = resourcesProvider;
    memories.softBodyStates.resourcesProvider = resourcesProvider;;

    resourcesProvider->resources->memoryManager->allocate();
  }

  utils::opencl::Memory* GPU::defineHostMemory(
    const std::string &name,
    size_t size,
    void *hostPtr,
    size_t allocationCount
  ) {
    return resourcesProvider->resources->memoryManager->define(name, size, hostPtr, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, allocationCount);
  }

  utils::opencl::Memory* GPU::defineGPUMemory(
    const std::string &name,
    size_t size,
    size_t allocationCount
  ) {
    return resourcesProvider->resources->memoryManager->define(name, size, nullptr, CL_MEM_READ_WRITE, allocationCount);
  }
}
