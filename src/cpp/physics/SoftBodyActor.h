#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "Actor.h"

namespace alcube::physics {
  class SoftBodyActor : public Actor {
    public:
      explicit SoftBodyActor();
      gpu::dtos::SoftBody subPhysicalQuantity;
  };
}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
