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
      if (!this->isAlive.get()) {
        this->allocationRange->deallocate();
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

  unsigned short Actor::getSubIndex() {
    return (unsigned short)subAllocationRange->getIndex();
  }
}