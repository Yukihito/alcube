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
    T** entities;
    explicit ActorResource(unsigned int size, utils::MemoryPool<utils::AllocationRange>* allocationRangeMemoryPool);
  };

  template <class T>
  ActorResource<T>::ActorResource(unsigned int size, utils::MemoryPool<utils::AllocationRange>* allocationRangeMemoryPool) {
    allocationRange = allocationRangeMemoryPool->get();
    allocationRange->init(0, size, allocationRangeMemoryPool);
    memoryPool = new utils::MemoryPool<T>(size);
    entities = new T*[size];
  }

  struct ActorResources {
    utils::AllocationRange* allocationRange;
    Actor** entities;
    ActorResource<fluid::Actor>* fluidResource;
    ActorResource<softbody::Actor>* softbodyResource;
    ActorResource<softbody::Spring>* springResource;
    utils::MemoryPool<utils::AllocationRange>* allocationRangeMemoryPool;

    explicit ActorResources(unsigned int size);
  };
}

#endif //ALCUBE_ACTOR_RESOURCES_H
