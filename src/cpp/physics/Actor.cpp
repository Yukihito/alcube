#include "Actor.h"

namespace alcube::physics {
  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange,
    utils::AllocationRange* subAllocationRange,
    Actor** actors
  ) {
    this->allocationRange = allocationRange;
    this->subAllocationRange = subAllocationRange;
    subAllocationRange->syncDeallocation(allocationRange);
    actors[this->allocationRange->getIndex()] = this;
    afterMove.f = [&]{
      actors[this->allocationRange->getIndex()] = this;
    };
    this->allocationRange->onAfterMove.subscribe(&afterMove);
    this->subAllocationRange->onAfterMove.subscribe(&afterMoveSub);

    beforeGc.f = [&]{
      if (!this->isAlive.get()) {
        this->allocationRange->deallocate();
        this->allocationRange->onBeforeGc.unsubscribe(&beforeGc);
        this->allocationRange->onAfterMove.unsubscribe(&afterMove);
        this->subAllocationRange->onAfterMove.unsubscribe(&afterMoveSub);
      }
    };
    this->allocationRange->onBeforeGc.subscribe(&beforeGc);

    this->actorStruct.init(gpuAccessor->dtos.hostActors, gpuAccessor->dtos.actors, allocationRange);
    cl_float3 vec3Zero = {0.0f, 0.0f, 0.0f};
    cl_float4 quatIdent = {0.0f, 0.0f, 0.0f, 1.0f};
    INIT_GPU_BASED_ACTOR_PROPERTY(type, 0)
    INIT_GPU_BASED_ACTOR_PROPERTY(radius, 1.0f);
    INIT_GPU_BASED_ACTOR_PROPERTY(mass, 1.0f);
    INIT_GPU_BASED_ACTOR_PROPERTY(position, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(rotation, quatIdent);
    INIT_GPU_BASED_ACTOR_PROPERTY(linearMomentum, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(angularMomentum, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(isAlive, true);
    INIT_GPU_BASED_REFERENCE(gpu::dtos::Actor, actorStruct, subIndex, subAllocationRange);
  }

  unsigned short Actor::getIndex() {
    return (unsigned short)allocationRange->getIndex();
  }
}