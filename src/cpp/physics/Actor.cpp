#include "Actor.h"

namespace alcube::physics {
  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange,
    utils::AllocationRange* subAllocationRange,
    Actor** entities
  ) {
    this->allocationRange = allocationRange;
    this->subAllocationRange = subAllocationRange;

    this->allocationRange->deallocateOn([&]{ return !this->isAlive.get(); });
    this->subAllocationRange->deallocateOn([&]{ return !this->isAlive.get(); });

    this->allocation.init(allocationRange, gpuAccessor->dtos.hostActors);
    this->entityAllocation.init(allocationRange, entities);
    this->entityAllocation.set(this);

    cl_float3 vec3Zero = {0.0f, 0.0f, 0.0f};
    cl_float4 quatIdent = {0.0f, 0.0f, 0.0f, 1.0f};
    INIT_GPU_BASED_ACTOR_PROPERTY(unsigned short, type, 0);
    INIT_GPU_BASED_ACTOR_PROPERTY(float, radius, 1.0f);
    INIT_GPU_BASED_ACTOR_PROPERTY(float, mass, 1.0f);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float3, position, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float4, rotation, quatIdent);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float3, linearMomentum, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(cl_float3, angularMomentum, vec3Zero);
    INIT_GPU_BASED_ACTOR_PROPERTY(int, isAlive, true);
    INIT_GPU_BASED_REFERENCE(allocation, subIndex, subAllocationRange);
  }
}