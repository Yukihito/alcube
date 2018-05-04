#ifndef ALCUBE_PHYSICS_ACTORFACTORY_H
#define ALCUBE_PHYSICS_ACTORFACTORY_H

#include "../utils/ResourceAllocation.h"
#include "../utils/MemoryPool.h"
#include "../gpu/GPUAccessor.h"
#include "fluid/Actor.h"
#include "softbody/Actor.h"

namespace alcube::physics {

  template <class T>
  struct ActorResource {
    utils::AllocationRange* subAllocationRange;
    utils::MemoryPool<T>* memoryPool;
    explicit ActorResource(unsigned int size);
  };

  template <class T>
  ActorResource<T>::ActorResource(unsigned int size) {
    subAllocationRange = new utils::AllocationRange(0, size);
    memoryPool = new utils::MemoryPool<T>(size);
  }

  class ActorFactory {
    public:
      explicit ActorFactory(
        unsigned int size,
        gpu::GPUAccessor* gpuAccessor
      );

      fluid::Actor* createFluid();
      softbody::Actor* createSoftbody();
    private:
      utils::AllocationRange* allocationRange;
      ActorResource<fluid::Actor>* fluidResource;
      ActorResource<softbody::Actor>* softbodyResource;
      gpu::GPUAccessor* gpuAccessor;
  };
}

#endif //ALCUBE_PHYSICS_ACTORFACTORY_H
