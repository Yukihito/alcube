#ifndef ALCUBE_PHYSICS_ACTOR_H
#define ALCUBE_PHYSICS_ACTOR_H

#include "../gpu/dtos.h"
#include "../gpu/GPUAccessor.h"
#include "../utils/ResourceAllocation.h"
#include "../utils/GPUBasedProperty.h"

namespace alcube::physics {
  enum ActorType {
    SOFT_BODY = 0,
    FACE,
    SPHERICAL_SHELL,
    FLUID
  };

  class Actor {
    public:
      utils::AllocationRange* allocationRange = nullptr;
      utils::AllocationRange* subAllocationRange = nullptr;
      utils::GPUBasedProperty<gpu::dtos::Actor, unsigned short> type;
      utils::GPUBasedProperty<gpu::dtos::Actor, float> radius;
      utils::GPUBasedProperty<gpu::dtos::Actor, float> mass;
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float3> position;
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float4> rotation;
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float3> linearMomentum;
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float3> angularMomentum;
      utils::GPUBasedProperty<gpu::dtos::Actor, int> isAlive;
      utils::GPUBasedReference<gpu::dtos::Actor> subIndex;
      virtual void beforeWrite() = 0;
      explicit Actor();
    protected:
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange,
        Actor** actors
      );
    private:
      utils::ResourceAllocation<gpu::dtos::Actor> allocation;
      utils::EventHandler<utils::AllocationMoveEvent> moveEventHandler = {};
      utils::EventHandler<utils::DeallocationEvent> deallocationEventHandler = {};
      Actor** actors;
  };
}
#define INIT_GPU_BASED_ACTOR_PROPERTY(typeName, propName, value) { INIT_GPU_BASED_PROPERTY(typeName, allocation, propName); (propName).set(value); }

#endif //ALCUBE_PHYSICS_ACTOR_H
