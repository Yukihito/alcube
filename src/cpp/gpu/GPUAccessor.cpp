#include "GPUAccessor.h"

namespace alcube::gpu {
  using namespace utils::opencl::kernelargs;
  namespace memories {
    dtos::Actor* Actor::at(int i) {
      return &dto[i];
    }

    void Actor::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Actor::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Actor::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Actor::read(Actor* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Actor::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void ActorState::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void ActorState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void ActorState::read(ActorState* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void ActorState::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void Constants::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Constants::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Constants::read(Constants* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Constants::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void Constants::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::Fluid* Fluid::at(int i) {
      return &dto[i];
    }

    void Fluid::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Fluid::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Fluid::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Fluid::read(Fluid* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Fluid::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void Fluid::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::FluidSettings* FluidSettings::at(int i) {
      return &dto[i];
    }

    void FluidSettings::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void FluidSettings::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void FluidSettings::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void FluidSettings::read(FluidSettings* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void FluidSettings::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void FluidSettings::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::Grid* Grid::at(int i) {
      return &dto[i];
    }

    void Grid::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Grid::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Grid::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Grid::read(Grid* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Grid::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void GridAndActorRelation::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void GridAndActorRelation::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void GridAndActorRelation::read(GridAndActorRelation* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void GridAndActorRelation::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void Intersection::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Intersection::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Intersection::read(Intersection* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Intersection::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void Intersection::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::Renderer* Renderer::at(int i) {
      return &dto[i];
    }

    void Renderer::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Renderer::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Renderer::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Renderer::read(Renderer* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Renderer::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void Renderer::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::SoftBody* SoftBody::at(int i) {
      return &dto[i];
    }

    void SoftBody::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void SoftBody::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void SoftBody::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void SoftBody::read(SoftBody* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void SoftBody::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void SoftBody::setCount(size_t count) {
      this->memory->count = count;
    }

    dtos::Spring* Spring::at(int i) {
      return &dto[i];
    }

    void Spring::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Spring::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Spring::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Spring::read(Spring* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Spring::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void SpringState::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void SpringState::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void SpringState::read(SpringState* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void SpringState::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void CharMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void CharMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void CharMemory::read(CharMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void CharMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void DoubleMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void DoubleMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void DoubleMemory::read(DoubleMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void DoubleMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void FloatMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void FloatMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void FloatMemory::read(FloatMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void FloatMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void FloatMemory::setCount(size_t count) {
      this->memory->count = count;
    }

    cl_float16* Float16Memory::at(int i) {
      return &dto[i];
    }

    void Float16Memory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Float16Memory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Float16Memory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Float16Memory::read(Float16Memory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Float16Memory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void Float16Memory::setCount(size_t count) {
      this->memory->count = count;
    }

    cl_float3* Float3Memory::at(int i) {
      return &dto[i];
    }

    void Float3Memory::write() {
      resourcesProvider->queue->write(this->memory);
    }

    void Float3Memory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Float3Memory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Float3Memory::read(Float3Memory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Float3Memory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void Float4Memory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void Float4Memory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void Float4Memory::read(Float4Memory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void Float4Memory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void IntMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void IntMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void IntMemory::read(IntMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void IntMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void LongMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void LongMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void LongMemory::read(LongMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void LongMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void ShortMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void ShortMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void ShortMemory::read(ShortMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void ShortMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void UcharMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void UcharMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void UcharMemory::read(UcharMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void UcharMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void UintMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void UintMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void UintMemory::read(UintMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void UintMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void UlongMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void UlongMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void UlongMemory::read(UlongMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void UlongMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
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

    void UshortMemory::write(size_t offset) {
      resourcesProvider->queue->write(this->memory, offset);
    }

    void UshortMemory::zeroFill() {
      resourcesProvider->queue->pushZeroFill(this->memory);
    }

    void UshortMemory::read(UshortMemory* dst) {
      resourcesProvider->queue->read(this->memory, dst->dto);
    }

    void UshortMemory::readRange(size_t min, size_t length) {
      resourcesProvider->queue->readRange(this->memory, this->dto, min, length);
    }

    void UshortMemory::setCount(size_t count) {
      this->memory->count = count;
    }
  }

  void Kernels::initGridAndActorRelations(
    unsigned int workSize,
    memories::GridAndActorRelation& relations,
    unsigned int gridIndex,
    unsigned short actorIndex
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.initGridAndActorRelations, {workSize}, {
      memArg(relations.memory),
      uintArg(gridIndex),
      ushortArg(actorIndex)
    });
  }

  void Kernels::fillGridIndex(
    unsigned int workSize,
    memories::Constants& constants,
    memories::ActorState& actorStates,
    memories::GridAndActorRelation& relations
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.fillGridIndex, {workSize}, {
      memArg(constants.memory),
      memArg(actorStates.memory),
      memArg(relations.memory)
    });
  }

  void Kernels::merge(
    unsigned int workSize,
    memories::GridAndActorRelation& relations,
    unsigned int distance
  ) {
    if (workSize == 0) {
        return;
    }
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
    if (workSize == 0) {
        return;
    }
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
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.setGridRelationIndexRange, {workSize}, {
      memArg(relations.memory),
      memArg(gridStartIndices.memory),
      memArg(gridEndIndices.memory),
      uintArg(actorCount)
    });
  }

  void Kernels::inputPhysicsConstants(
    unsigned int workSize,
    memories::Constants& constants,
    memories::Grid& grid,
    float gravityAcceleration,
    float deltaTime,
    float sphericalShellRadius
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputPhysicsConstants, {workSize}, {
      memArg(constants.memory),
      memArg(grid.memory),
      floatArg(gravityAcceleration),
      floatArg(deltaTime),
      floatArg(sphericalShellRadius)
    });
  }

  void Kernels::inputFluidConstants(
    unsigned int workSize,
    memories::Constants& constants,
    memories::FluidSettings& fluidSettings
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputFluidConstants, {workSize}, {
      memArg(constants.memory),
      memArg(fluidSettings.memory)
    });
  }

  void Kernels::inputSoftbodyConstants(
    unsigned int workSize,
    memories::Constants& constants,
    float splitDeltaTime
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputSoftbodyConstants, {workSize}, {
      memArg(constants.memory),
      floatArg(splitDeltaTime)
    });
  }

  void Kernels::inputActors(
    unsigned int workSize,
    memories::Actor& hostActors,
    memories::ActorState& actorStates,
    unsigned short offset
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputActors, {workSize}, {
      memArg(hostActors.memory),
      memArg(actorStates.memory),
      ushortArg(offset)
    });
  }

  void Kernels::inputSoftBodies(
    unsigned int workSize,
    memories::SoftBody& hostSoftBodies,
    memories::SoftBody& softBodies,
    unsigned short offset
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputSoftBodies, {workSize}, {
      memArg(hostSoftBodies.memory),
      memArg(softBodies.memory),
      ushortArg(offset)
    });
  }

  void Kernels::inputSprings(
    unsigned int workSize,
    memories::Spring& springs,
    memories::SpringState& springStates,
    unsigned int offset
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputSprings, {workSize}, {
      memArg(springs.memory),
      memArg(springStates.memory),
      uintArg(offset)
    });
  }

  void Kernels::inputFluids(
    unsigned int workSize,
    memories::Fluid& hostFluids,
    memories::Fluid& fluids,
    unsigned short offset
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputFluids, {workSize}, {
      memArg(hostFluids.memory),
      memArg(fluids.memory),
      ushortArg(offset)
    });
  }

  void Kernels::moveFluid(
    unsigned int workSize,
    memories::Fluid& fluids,
    memories::ActorState& actorStates,
    memories::Constants& constants
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.moveFluid, {workSize}, {
      memArg(fluids.memory),
      memArg(actorStates.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::calcSpringImpulses(
    unsigned int workSize,
    memories::Constants& constants,
    memories::SpringState& springStates,
    memories::ActorState& actorStates
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.calcSpringImpulses, {workSize}, {
      memArg(constants.memory),
      memArg(springStates.memory),
      memArg(actorStates.memory)
    });
  }

  void Kernels::updateBySpringImpulse(
    unsigned int workSize,
    memories::Constants& constants,
    memories::SoftBody& softBodies,
    memories::ActorState& actorStates,
    memories::SpringState& springStates
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateBySpringImpulse, {workSize}, {
      memArg(constants.memory),
      memArg(softBodies.memory),
      memArg(actorStates.memory),
      memArg(springStates.memory)
    });
  }

  void Kernels::postProcessing(
    unsigned int workSize,
    memories::Constants& constants,
    memories::ActorState& actorStates
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.postProcessing, {workSize}, {
      memArg(constants.memory),
      memArg(actorStates.memory)
    });
  }

  void Kernels::collectIntersections(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::GridAndActorRelation& relations,
    memories::UintMemory& gridStartIndices,
    memories::UintMemory& gridEndIndices,
    memories::Constants& constants
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.collectIntersections, {workSize}, {
      memArg(actorStates.memory),
      memArg(relations.memory),
      memArg(gridStartIndices.memory),
      memArg(gridEndIndices.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::outputActors(
    unsigned int workSize,
    memories::Actor& actors,
    memories::ActorState& actorStates
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.outputActors, {workSize}, {
      memArg(actors.memory),
      memArg(actorStates.memory)
    });
  }

  void Kernels::collectCollisions(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::SoftBody& softBodies
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.collectCollisions, {workSize}, {
      memArg(actorStates.memory),
      memArg(softBodies.memory)
    });
  }

  void Kernels::updateByConstraintImpulse(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::SoftBody& softBodies
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateByConstraintImpulse, {workSize}, {
      memArg(actorStates.memory),
      memArg(softBodies.memory)
    });
  }

  void Kernels::updateDensityAndPressure(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::Fluid& fluids,
    memories::Constants& constants
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateDensityAndPressure, {workSize}, {
      memArg(actorStates.memory),
      memArg(fluids.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::updateFluidForce(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::Fluid& fluids,
    memories::Constants& constants
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateFluidForce, {workSize}, {
      memArg(actorStates.memory),
      memArg(fluids.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::updateByFrictionalImpulse(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::SoftBody& softBodies
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateByFrictionalImpulse, {workSize}, {
      memArg(actorStates.memory),
      memArg(softBodies.memory)
    });
  }

  void Kernels::initStepVariables(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::Constants& constants
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.initStepVariables, {workSize}, {
      memArg(actorStates.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::updateByPenaltyImpulse(
    unsigned int workSize,
    memories::ActorState& actorStates,
    memories::SoftBody& softBodies,
    memories::Constants& constants
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateByPenaltyImpulse, {workSize}, {
      memArg(actorStates.memory),
      memArg(softBodies.memory),
      memArg(constants.memory)
    });
  }

  void Kernels::inputRenderers(
    unsigned int workSize,
    memories::Renderer& hostRenderers,
    memories::Renderer& renderers,
    memories::Float3Memory& hostColors,
    memories::Float3Memory& colors,
    unsigned int offset
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.inputRenderers, {workSize}, {
      memArg(hostRenderers.memory),
      memArg(renderers.memory),
      memArg(hostColors.memory),
      memArg(colors.memory),
      uintArg(offset)
    });
  }

  void Kernels::updateDrawingBuffer(
    unsigned int workSize,
    memories::Float3Memory& positions,
    memories::Float3Memory& colors,
    memories::Float4Memory& rotations0,
    memories::Float4Memory& rotations1,
    memories::Float4Memory& rotations2,
    memories::Float4Memory& rotations3,
    memories::ActorState& actorStates,
    memories::Renderer& renderers,
    unsigned int offset
  ) {
    if (workSize == 0) {
        return;
    }
    queue->push(rawKernels.updateDrawingBuffer, {workSize}, {
      memArg(positions.memory),
      memArg(colors.memory),
      memArg(rotations0.memory),
      memArg(rotations1.memory),
      memArg(rotations2.memory),
      memArg(rotations3.memory),
      memArg(actorStates.memory),
      memArg(renderers.memory),
      uintArg(offset)
    });
  }

  GPUAccessor::GPUAccessor(
    utils::opencl::ResourcesProvider *resourcesProvider,
    unsigned int maxActorCount,
    unsigned int maxActorCountForBitonicSort,
    unsigned int maxSpringCount,
    unsigned int drawingBufferSize,
    unsigned int allGridCount
  ) {
    this->resourcesProvider = resourcesProvider;
    cl_program program = resourcesProvider->programFactory->create("../src/kernels/generated-code/all.cl");
    kernels.queue = resourcesProvider->queue;

    kernels.rawKernels.initGridAndActorRelations = resourcesProvider->kernelFactory->create(program, "initGridAndActorRelations");
    kernels.rawKernels.fillGridIndex = resourcesProvider->kernelFactory->create(program, "fillGridIndex");
    kernels.rawKernels.merge = resourcesProvider->kernelFactory->create(program, "merge");
    kernels.rawKernels.bitonic = resourcesProvider->kernelFactory->create(program, "bitonic");
    kernels.rawKernels.setGridRelationIndexRange = resourcesProvider->kernelFactory->create(program, "setGridRelationIndexRange");
    kernels.rawKernels.inputPhysicsConstants = resourcesProvider->kernelFactory->create(program, "inputPhysicsConstants");
    kernels.rawKernels.inputFluidConstants = resourcesProvider->kernelFactory->create(program, "inputFluidConstants");
    kernels.rawKernels.inputSoftbodyConstants = resourcesProvider->kernelFactory->create(program, "inputSoftbodyConstants");
    kernels.rawKernels.inputActors = resourcesProvider->kernelFactory->create(program, "inputActors");
    kernels.rawKernels.inputSoftBodies = resourcesProvider->kernelFactory->create(program, "inputSoftBodies");
    kernels.rawKernels.inputSprings = resourcesProvider->kernelFactory->create(program, "inputSprings");
    kernels.rawKernels.inputFluids = resourcesProvider->kernelFactory->create(program, "inputFluids");
    kernels.rawKernels.moveFluid = resourcesProvider->kernelFactory->create(program, "moveFluid");
    kernels.rawKernels.calcSpringImpulses = resourcesProvider->kernelFactory->create(program, "calcSpringImpulses");
    kernels.rawKernels.updateBySpringImpulse = resourcesProvider->kernelFactory->create(program, "updateBySpringImpulse");
    kernels.rawKernels.postProcessing = resourcesProvider->kernelFactory->create(program, "postProcessing");
    kernels.rawKernels.collectIntersections = resourcesProvider->kernelFactory->create(program, "collectIntersections");
    kernels.rawKernels.outputActors = resourcesProvider->kernelFactory->create(program, "outputActors");
    kernels.rawKernels.collectCollisions = resourcesProvider->kernelFactory->create(program, "collectCollisions");
    kernels.rawKernels.updateByConstraintImpulse = resourcesProvider->kernelFactory->create(program, "updateByConstraintImpulse");
    kernels.rawKernels.updateDensityAndPressure = resourcesProvider->kernelFactory->create(program, "updateDensityAndPressure");
    kernels.rawKernels.updateFluidForce = resourcesProvider->kernelFactory->create(program, "updateFluidForce");
    kernels.rawKernels.updateByFrictionalImpulse = resourcesProvider->kernelFactory->create(program, "updateByFrictionalImpulse");
    kernels.rawKernels.initStepVariables = resourcesProvider->kernelFactory->create(program, "initStepVariables");
    kernels.rawKernels.updateByPenaltyImpulse = resourcesProvider->kernelFactory->create(program, "updateByPenaltyImpulse");
    kernels.rawKernels.inputRenderers = resourcesProvider->kernelFactory->create(program, "inputRenderers");
    kernels.rawKernels.updateDrawingBuffer = resourcesProvider->kernelFactory->create(program, "updateDrawingBuffer");

    dtos.grid = new dtos::Grid();
    dtos.fluidSettings = new dtos::FluidSettings();
    dtos.constants = new dtos::Constants();
    dtos.hostActors = new dtos::Actor[maxActorCount];
    dtos.actors = new dtos::Actor[maxActorCount];
    dtos.actorStates = new dtos::ActorState[maxActorCount];
    dtos.hostSoftBodies = new dtos::SoftBody[maxActorCount];
    dtos.softBodies = new dtos::SoftBody[maxActorCount];
    dtos.springs = new dtos::Spring[maxSpringCount];
    dtos.springStates = new dtos::SpringState[maxSpringCount];
    dtos.hostFluids = new dtos::Fluid[maxActorCount];
    dtos.fluids = new dtos::Fluid[maxActorCount];
    dtos.gridAndActorRelations = new dtos::GridAndActorRelation[maxActorCountForBitonicSort];
    dtos.gridStartIndices = new unsigned int[allGridCount];
    dtos.gridEndIndices = new unsigned int[allGridCount];
    dtos.positions = new cl_float3[drawingBufferSize];
    dtos.rotations0 = new cl_float4[drawingBufferSize];
    dtos.rotations1 = new cl_float4[drawingBufferSize];
    dtos.rotations2 = new cl_float4[drawingBufferSize];
    dtos.rotations3 = new cl_float4[drawingBufferSize];
    dtos.hostColors = new cl_float3[drawingBufferSize];
    dtos.colors = new cl_float3[drawingBufferSize];
    dtos.hostRenderers = new dtos::Renderer[drawingBufferSize];
    dtos.renderers = new dtos::Renderer[drawingBufferSize];

    memories.grid.memory = defineHostMemory("grid", sizeof(dtos::Grid), dtos.grid, 1);
    memories.fluidSettings.memory = defineHostMemory("fluidSettings", sizeof(dtos::FluidSettings), dtos.fluidSettings, 1);
    memories.constants.memory = defineGPUMemory("constants", sizeof(dtos::Constants), 1);
    memories.hostActors.memory = defineHostMemory("hostActors", sizeof(dtos::Actor), dtos.hostActors, maxActorCount);
    memories.actors.memory = defineGPUMemory("actors", sizeof(dtos::Actor), maxActorCount);
    memories.actorStates.memory = defineGPUMemory("actorStates", sizeof(dtos::ActorState), maxActorCount);
    memories.hostSoftBodies.memory = defineHostMemory("hostSoftBodies", sizeof(dtos::SoftBody), dtos.hostSoftBodies, maxActorCount);
    memories.softBodies.memory = defineGPUMemory("softBodies", sizeof(dtos::SoftBody), maxActorCount);
    memories.springs.memory = defineHostMemory("springs", sizeof(dtos::Spring), dtos.springs, maxSpringCount);
    memories.springStates.memory = defineGPUMemory("springStates", sizeof(dtos::SpringState), maxSpringCount);
    memories.hostFluids.memory = defineHostMemory("hostFluids", sizeof(dtos::Fluid), dtos.hostFluids, maxActorCount);
    memories.fluids.memory = defineGPUMemory("fluids", sizeof(dtos::Fluid), maxActorCount);
    memories.gridAndActorRelations.memory = defineGPUMemory("gridAndActorRelations", sizeof(dtos::GridAndActorRelation), maxActorCountForBitonicSort);
    memories.gridStartIndices.memory = defineGPUMemory("gridStartIndices", sizeof(unsigned int), allGridCount);
    memories.gridEndIndices.memory = defineGPUMemory("gridEndIndices", sizeof(unsigned int), allGridCount);
    memories.positions.memory = defineGPUMemory("positions", sizeof(cl_float3), drawingBufferSize);
    memories.rotations0.memory = defineGPUMemory("rotations0", sizeof(cl_float4), drawingBufferSize);
    memories.rotations1.memory = defineGPUMemory("rotations1", sizeof(cl_float4), drawingBufferSize);
    memories.rotations2.memory = defineGPUMemory("rotations2", sizeof(cl_float4), drawingBufferSize);
    memories.rotations3.memory = defineGPUMemory("rotations3", sizeof(cl_float4), drawingBufferSize);
    memories.hostColors.memory = defineHostMemory("hostColors", sizeof(cl_float3), dtos.hostColors, drawingBufferSize);
    memories.colors.memory = defineGPUMemory("colors", sizeof(cl_float3), drawingBufferSize);
    memories.hostRenderers.memory = defineHostMemory("hostRenderers", sizeof(dtos::Renderer), dtos.hostRenderers, drawingBufferSize);
    memories.renderers.memory = defineGPUMemory("renderers", sizeof(dtos::Renderer), drawingBufferSize);

    memories.grid.dto = dtos.grid;
    memories.fluidSettings.dto = dtos.fluidSettings;
    memories.constants.dto = dtos.constants;
    memories.hostActors.dto = dtos.hostActors;
    memories.actors.dto = dtos.actors;
    memories.actorStates.dto = dtos.actorStates;
    memories.hostSoftBodies.dto = dtos.hostSoftBodies;
    memories.softBodies.dto = dtos.softBodies;
    memories.springs.dto = dtos.springs;
    memories.springStates.dto = dtos.springStates;
    memories.hostFluids.dto = dtos.hostFluids;
    memories.fluids.dto = dtos.fluids;
    memories.gridAndActorRelations.dto = dtos.gridAndActorRelations;
    memories.gridStartIndices.dto = dtos.gridStartIndices;
    memories.gridEndIndices.dto = dtos.gridEndIndices;
    memories.positions.dto = dtos.positions;
    memories.rotations0.dto = dtos.rotations0;
    memories.rotations1.dto = dtos.rotations1;
    memories.rotations2.dto = dtos.rotations2;
    memories.rotations3.dto = dtos.rotations3;
    memories.hostColors.dto = dtos.hostColors;
    memories.colors.dto = dtos.colors;
    memories.hostRenderers.dto = dtos.hostRenderers;
    memories.renderers.dto = dtos.renderers;

    memories.grid.resourcesProvider = resourcesProvider;
    memories.fluidSettings.resourcesProvider = resourcesProvider;
    memories.constants.resourcesProvider = resourcesProvider;
    memories.hostActors.resourcesProvider = resourcesProvider;
    memories.actors.resourcesProvider = resourcesProvider;
    memories.actorStates.resourcesProvider = resourcesProvider;
    memories.hostSoftBodies.resourcesProvider = resourcesProvider;
    memories.softBodies.resourcesProvider = resourcesProvider;
    memories.springs.resourcesProvider = resourcesProvider;
    memories.springStates.resourcesProvider = resourcesProvider;
    memories.hostFluids.resourcesProvider = resourcesProvider;
    memories.fluids.resourcesProvider = resourcesProvider;
    memories.gridAndActorRelations.resourcesProvider = resourcesProvider;
    memories.gridStartIndices.resourcesProvider = resourcesProvider;
    memories.gridEndIndices.resourcesProvider = resourcesProvider;
    memories.positions.resourcesProvider = resourcesProvider;
    memories.rotations0.resourcesProvider = resourcesProvider;
    memories.rotations1.resourcesProvider = resourcesProvider;
    memories.rotations2.resourcesProvider = resourcesProvider;
    memories.rotations3.resourcesProvider = resourcesProvider;
    memories.hostColors.resourcesProvider = resourcesProvider;
    memories.colors.resourcesProvider = resourcesProvider;
    memories.hostRenderers.resourcesProvider = resourcesProvider;
    memories.renderers.resourcesProvider = resourcesProvider;;

    resourcesProvider->resources->memoryManager->allocate();
  }

  utils::opencl::Memory* GPUAccessor::defineHostMemory(
    const std::string &name,
    size_t size,
    void *hostPtr,
    size_t allocationCount
  ) {
    return resourcesProvider->resources->memoryManager->define(name, size, hostPtr, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, allocationCount);
  }

  utils::opencl::Memory* GPUAccessor::defineGPUMemory(
    const std::string &name,
    size_t size,
    size_t allocationCount
  ) {
    return resourcesProvider->resources->memoryManager->define(name, size, nullptr, CL_MEM_READ_WRITE, allocationCount);
  }
}
