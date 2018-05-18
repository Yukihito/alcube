#include "Actor.h"

namespace alcube::physics {
  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange
  ) {
    this->allocationRange = allocationRange;
    this->allocationRange->onBeforeMove.subscribe([&]{
      this->temporal = *this->actor.getPtr();
    });
    this->allocationRange->onAfterMove.subscribe([&]{
      this->hostActor.getPtr()[0] = temporal;
    });
    this->allocationRange->onBeforeGc.subscribe([&]{
      if (!this->actor.getPtr()->isAlive) {
        this->allocationRange->deallocate();
      }
    });
    this->hostActor.init(allocationRange, gpuAccessor->dtos.hostActors);
    this->actor.init(allocationRange, gpuAccessor->dtos.actors);
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
    return hostActor.getPtr()->subIndex;
  }

  gpu::dtos::Actor* Actor::getDto() {
    return this->hostActor.getPtr();
  }
}