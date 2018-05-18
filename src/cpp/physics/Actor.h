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
      utils::GPUBasedStruct<gpu::dtos::Actor> actorStruct = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, unsigned short> type = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, float> radius = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, float> mass = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float3> position = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float4> rotation = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float3> linearMomentum = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, cl_float3> angularMomentum = {};
      utils::GPUBasedProperty<gpu::dtos::Actor, int> isAlive = {};
      utils::GPUBasedReference<gpu::dtos::Actor> subIndex = {};
      gpu::dtos::Actor* getDto();
      unsigned short getIndex();
      unsigned short getSubIndex();
      virtual void beforeWrite() = 0;
    protected:
      utils::ResourceAllocation<gpu::dtos::Actor> hostActor = {};
      utils::ResourceAllocation<gpu::dtos::Actor> actor = {};

      virtual void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange
      );
      virtual void updateIndex() = 0;
      utils::AllocationRange* allocationRange = nullptr;
      utils::AllocationRange* subAllocationRange = nullptr;

    private:
      utils::EventHandler beforeGc;
  };
}

#define INIT_GPU_BASED_ACTOR_PROPERTY(propName) { INIT_GPU_BASED_PROPERTY(gpu::dtos::Actor, actorStruct, propName); }

#endif //ALCUBE_PHYSICS_ACTOR_H
