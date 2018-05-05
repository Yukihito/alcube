#ifndef ALCUBE_PHYSICS_ACTOR_FACTORY_H
#define ALCUBE_PHYSICS_ACTOR_FACTORY_H

#include "../gpu/GPUAccessor.h"
#include "fluid/Actor.h"
#include "softbody/Actor.h"
#include "softbody/Spring.h"
#include "ActorResources.h"

namespace alcube::physics {
  class ActorFactory {
    public:
      explicit ActorFactory(
        ActorResources* actorResources,
        gpu::GPUAccessor* gpuAccessor
      );

      fluid::Actor* createFluid();
      softbody::Actor* createSoftbody();
      softbody::Spring* createSpring();
    private:
      ActorResources* actorResources;
      gpu::GPUAccessor* gpuAccessor;
  };
}

#endif //ALCUBE_PHYSICS_ACTOR_FACTORY_H
