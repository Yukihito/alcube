#include "ActorFactory.h"

namespace alcube::physics {
  ActorFactory::ActorFactory(
    unsigned int size,
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->gpuAccessor = gpuAccessor;
    allocationRange = new utils::AllocationRange(0, size);
    fluidResource = new ActorResource<fluid::Actor>(size);
    softbodyResource = new ActorResource<softbody::Actor>(size);
  }


  fluid::Actor* ActorFactory::createFluid() {
    auto entity = fluidResource->memoryPool->get();
    entity->init(gpuAccessor, allocationRange->allocate(1), fluidResource->subAllocationRange->allocate(1));
    return entity;
  }

  softbody::Actor* ActorFactory::createSoftbody() {
    auto entity = softbodyResource->memoryPool->get();
    entity->init(gpuAccessor, allocationRange->allocate(1), softbodyResource->subAllocationRange->allocate(1));
    return entity;
  }
}
