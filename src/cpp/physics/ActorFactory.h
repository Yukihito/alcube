#ifndef ALCUBE_PHYSICS_ACTORFACTORY_H
#define ALCUBE_PHYSICS_ACTORFACTORY_H

#include "../gpu/GPUAccessor.h"
#include "fluid/Actor.h"
#include "softbody/Actor.h"
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
    private:
      ActorResources* actorResources;
      gpu::GPUAccessor* gpuAccessor;
  };
}

#endif //ALCUBE_PHYSICS_ACTORFACTORY_H
