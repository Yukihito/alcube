#ifndef ALCUBE_ACTORRESOURCES_H
#define ALCUBE_ACTORRESOURCES_H

#include "../utils/ResourceAllocation.h"
#include "../utils/MemoryPool.h"
#include "fluid/Actor.h"
#include "softbody/Actor.h"

namespace alcube::physics {
  template<class T>
  struct ActorResource {
    utils::AllocationRange *subAllocationRange;
    utils::MemoryPool <T> *memoryPool;

    explicit ActorResource(unsigned int size);
  };

  template <class T>
  ActorResource<T>::ActorResource(unsigned int size) {
    subAllocationRange = new utils::AllocationRange(0, size);
    memoryPool = new utils::MemoryPool<T>(size);
  }

  struct ActorResources {
    utils::AllocationRange* allocationRange;
    ActorResource<fluid::Actor>* fluidResource;
    ActorResource<softbody::Actor>* softbodyResource;

    explicit ActorResources(unsigned int size);
  };
}

#endif //ALCUBE_ACTORRESOURCES_H
