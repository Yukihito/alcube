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
      gpu::dtos::Actor* getDto();
      virtual unsigned short getIndex() = 0;
      unsigned short getSubIndex();
      virtual ~Actor() = default;
      virtual void beforeWrite() = 0;
    protected:
      utils::ResourceAllocation<gpu::dtos::Actor> hostActor = {};
      utils::ResourceAllocation<gpu::dtos::Actor> actor = {};
      utils::ResourceAllocation<gpu::dtos::ActorState> actorState = {};

      explicit Actor() = default;
      void init(gpu::GPUAccessor* gpuAccessor, utils::AllocationRange* allocationRange);
      virtual void updateIndex() = 0;
      utils::AllocationRange* allocationRange = nullptr;
      gpu::dtos::Actor temporal = {};
  };
}

#endif //ALCUBE_PHYSICS_ACTOR_H
