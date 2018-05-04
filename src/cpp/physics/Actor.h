#ifndef ALCUBE_PHYSICS_ACTOR_H
#define ALCUBE_PHYSICS_ACTOR_H

#include "../gpu/dtos.h"
#include "../gpu/GPUAccessor.h"
#include "../utils/ResourceAllocation.h"

namespace alcube::physics {
  enum ActorType {
    SOFT_BODY = 0,
    FACE,
    SPHERICAL_SHELL,
    FLUID
  };

  class Actor {
    public:
      gpu::dtos::PhysicalQuantity* getPhysicalQuantityDto();
      virtual unsigned short getIndex() = 0;
      virtual ~Actor() = default;
    protected:
      utils::ResourceAllocation<gpu::dtos::Actor>* actor = nullptr;
      utils::ResourceAllocation<gpu::dtos::PhysicalQuantity>* hostPhysicalQuantity = nullptr;
      utils::ResourceAllocation<gpu::dtos::ActorState>* actorState = nullptr;
      utils::ResourceAllocation<gpu::dtos::PhysicalQuantity>* physicalQuantity = nullptr;

      explicit Actor() = default;
      void init(gpu::GPUAccessor* gpuAccessor, utils::AllocationRange* allocationRange);
      virtual void updateIndex() = 0;
      utils::AllocationRange* allocationRange = nullptr;
  };
}


#endif //ALCUBE_PHYSICS_ACTOR_H
