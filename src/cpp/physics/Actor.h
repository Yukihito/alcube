#ifndef ALCUBE_ACTOR_H
#define ALCUBE_ACTOR_H

#include "../gpu/dtos.h"

namespace alcube::physics {
  enum ActorType {
    SOFT_BODY = 0,
    FACE,
    SPHERICAL_SHELL,
    FLUID
  };

  class Actor {
    public:
      unsigned short index;
      gpu::dtos::Actor actor;
      gpu::dtos::PhysicalQuantity physicalQuantity;
    protected:
      explicit Actor();
  };
}


#endif //ALCUBE_ACTOR_H
