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
      //gpu::dtos::Actor actor;
      //gpu::dtos::PhysicalQuantity physicalQuantity;
      gpu::dtos::Actor* getActorDto();
      gpu::dtos::PhysicalQuantity* getPhysicalQuantityDto();
      virtual unsigned short getIndex() = 0;
      virtual void setIndex(unsigned short index) = 0;
      unsigned short getSubIndex();
      void setSubIndex(unsigned short index);
      virtual ~Actor() = default;
    protected:
      explicit Actor() = default;
      void init(gpu::GPUAccessor* gpuAccessor, utils::AllocationRange* allocationRange);
      utils::ResourceAllocation<gpu::dtos::Actor>* actor = nullptr;
      utils::ResourceAllocation<gpu::dtos::PhysicalQuantity>* hostPhysicalQuantity = nullptr;
      utils::ResourceAllocation<gpu::dtos::ActorState>* actorState = nullptr;
      utils::ResourceAllocation<gpu::dtos::PhysicalQuantity>* physicalQuantity = nullptr;
    private:
      utils::AllocationRange* allocationRange = nullptr;
  };
}


#endif //ALCUBE_PHYSICS_ACTOR_H
