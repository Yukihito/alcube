#include "ActorFactory.h"

namespace alcube::physics {
  ActorFactory::ActorFactory(
    ActorResources* actorResources,
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->actorResources = actorResources;
    this->gpuAccessor = gpuAccessor;
  }

  fluid::Actor* ActorFactory::createFluid() {
    auto entity = actorResources->fluidResource->memoryPool->get();
    entity->init(gpuAccessor, actorResources->allocationRange->allocate(1), actorResources->fluidResource->subAllocationRange->allocate(1));
    return entity;
  }

  softbody::Actor* ActorFactory::createSoftbody() {
    auto entity = actorResources->softbodyResource->memoryPool->get();
    entity->init(gpuAccessor, actorResources->allocationRange->allocate(1), actorResources->softbodyResource->subAllocationRange->allocate(1));
    return entity;
  }
}
