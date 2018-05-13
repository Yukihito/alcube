#include "Actor.h"

namespace alcube::physics {
  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange
  ) {
    this->allocationRange = allocationRange;
    this->hostActor.init(allocationRange, gpuAccessor->dtos.hostActors);
    this->actorState.init(allocationRange, gpuAccessor->dtos.actorStates);
    this->hostActor.getPtr()->radius = 1.0f;
    this->hostActor.getPtr()->mass = 1.0f;
    this->hostActor.getPtr()->rotation = {0.0f, 0.0f, 0.0f, 1.0f};
    this->hostActor.getPtr()->angularMomentum = {0.0f, 0.0f, 0.0f};
    this->hostActor.getPtr()->linearMomentum = {0.0f, 0.0f, 0.0f};
    this->hostActor.getPtr()->position = {0.0f, 0.0f, 0.0f};
    this->hostActor.getPtr()->isAlive = true;
  }

  unsigned short Actor::getSubIndex() {
    return hostActor.getPtr()->subPhysicalQuantityIndex;
  }

  gpu::dtos::Actor* Actor::getDto() {
    return this->hostActor.getPtr();
  }
}