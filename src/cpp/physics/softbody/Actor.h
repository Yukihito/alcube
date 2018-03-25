#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "../Actor.h"

namespace alcube::physics::softbody {
  class Actor : public physics::Actor {
    public:
      explicit Actor();
      gpu::dtos::SoftBody subPhysicalQuantity;
  };
}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
