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
    auto entity = actorResources->fluidResource->memoryPool->allocate();
    entity->init(
      gpuAccessor,
      actorResources->allocationRange->allocate(1),
      actorResources->fluidResource->allocationRange->allocate(1),
      actorResources->entities,
      actorResources->fluidResource->entities
    );
    return entity;
  }

  softbody::Actor* ActorFactory::createSoftbody() {
    auto entity = actorResources->softbodyResource->memoryPool->allocate();
    entity->init(
      gpuAccessor,
      actorResources->allocationRange->allocate(1),
      actorResources->softbodyResource->allocationRange->allocate(1),
      actorResources->entities,
      actorResources->softbodyResource->entities
    );
    return entity;
  }

  softbody::Spring* ActorFactory::createSpring() {
    auto entity = actorResources->springResource->memoryPool->allocate();
    entity->init(
      gpuAccessor,
      actorResources->springResource->allocationRange->allocate(1),
      actorResources->springResource->entities
    );
    return entity;
  }
}
