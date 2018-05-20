#include "Actor.h"

namespace alcube::physics {
  Actor::Actor() {
    this->moveEventHandler.f = [&](utils::AllocationMoveEvent &e) {
      this->actors[e.dst] = this;
    };

    this->deallocationEventHandler.f = [&](utils::DeallocationEvent &e) {
      this->allocationRange->onMove.unsubscribe(this->moveEventHandler);
      this->allocationRange->onDeallocate.unsubscribe(this->deallocationEventHandler);
    };
  }

  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange,
    utils::AllocationRange* subAllocationRange,
    Actor** actors
  ) {
    this->allocationRange = allocationRange;
    this->subAllocationRange = subAllocationRange;
    this->allocationRange->deallocateOn([&]{ return !this->isAlive.get(); });
    this->subAllocationRange->deallocateOn([&]{ return !this->isAlive.get(); });
    this->allocation.init(allocationRange, gpuAccessor->dtos.hostActors);
    this->actors = actors;
    this->actors[this->allocationRange->getIndex()] = this;
    this->allocationRange->onMove.subscribe(moveEventHandler);
    this->allocationRange->onDeallocate.subscribe(deallocationEventHandler);
    cl_float3 vec3Zero = {0.0f, 0.0f, 0.0f};
    cl_float4 quatIdent = {0.0f, 0.0f, 0.0f, 1.0f};
    INIT_GPU_BASED_ACTOR_PROPERTY(unsigned short, type, 0)
    INIT_GPU_BASED_ACTOR_PROPERTY(float, radius, 1.0f);
    INIT_GPU_BASED_ACTOR_PROPERTY(float, mass, 1.0f);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float3, position, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float4, rotation, quatIdent);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float3, linearMomentum, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float3, angularMomentum, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(int, isAlive, true);
    INIT_GPU_BASED_REFERENCE(gpu::dtos::Actor, allocation, subIndex, subAllocationRange);
  }
}