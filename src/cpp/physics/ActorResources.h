#ifndef ALCUBE_ACTOR_RESOURCES_H
#define ALCUBE_ACTOR_RESOURCES_H

#include "../utils/ResourceAllocation.h"
#include "../utils/MemoryPool.h"
#include "fluid/Actor.h"
#include "softbody/Actor.h"
#include "softbody/Spring.h"

namespace alcube::physics {
  template<class T>
  struct ActorResource {
    utils::AllocationRange *allocationRange;
    utils::MemoryPool <T> *memoryPool;

    explicit ActorResource(unsigned int size);
  };

  template <class T>
  ActorResource<T>::ActorResource(unsigned int size) {
    allocationRange = new utils::AllocationRange(0, size);
    memoryPool = new utils::MemoryPool<T>(size);
  }

  struct ActorResources {
    utils::AllocationRange* allocationRange;
    ActorResource<fluid::Actor>* fluidResource;
    ActorResource<softbody::Actor>* softbodyResource;
    ActorResource<softbody::Spring>* springResource;

    explicit ActorResources(unsigned int size);
  };
}

#endif //ALCUBE_ACTOR_RESOURCES_H
