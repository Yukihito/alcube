#include "Actor.h"

namespace alcube::physics {
  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange,
    utils::AllocationRange* subAllocationRange
  ) {
    this->allocationRange = allocationRange;
    this->subAllocationRange = subAllocationRange;
    beforeGc.f = [&]{
      if (!this->actor.getPtr()->isAlive) {
        this->allocationRange->deallocate();
      }
    };
    this->allocationRange->onBeforeGc.subscribe(&beforeGc);
    this->hostActor.init(allocationRange, gpuAccessor->dtos.hostActors);
    this->actor.init(allocationRange, gpuAccessor->dtos.actors);
    this->actorStruct.init(gpuAccessor->dtos.hostActors, gpuAccessor->dtos.actors, allocationRange);
    INIT_GPU_BASED_ACTOR_PROPERTY(type)
    INIT_GPU_BASED_ACTOR_PROPERTY(radius);
    INIT_GPU_BASED_ACTOR_PROPERTY(mass);
    INIT_GPU_BASED_ACTOR_PROPERTY(position);
    INIT_GPU_BASED_ACTOR_PROPERTY(rotation);
    INIT_GPU_BASED_ACTOR_PROPERTY(linearMomentum);
    INIT_GPU_BASED_ACTOR_PROPERTY(angularMomentum);
    INIT_GPU_BASED_ACTOR_PROPERTY(isAlive);
    INIT_GPU_BASED_REFERENCE(gpu::dtos::Actor, actorStruct, subIndex, subAllocationRange);

    radius.set(1.0f);
    mass.set(1.0f);
    rotation.set({0.0f, 0.0f, 0.0f, 1.0f});
    angularMomentum.set({0.0f, 0.0f, 0.0f});
    linearMomentum.set({0.0f, 0.0f, 0.0f});
    position.set({0.0f, 0.0f, 0.0f});
    isAlive.set(true);
  }

  unsigned short Actor::getIndex() {
    return (unsigned short)allocationRange->getIndex();
  }

  unsigned short Actor::getSubIndex() {
    return (unsigned short)subAllocationRange->getIndex();
  }

  gpu::dtos::Actor* Actor::getDto() {
    return this->hostActor.getPtr();
  }
}