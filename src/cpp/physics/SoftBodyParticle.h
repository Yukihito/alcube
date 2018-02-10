#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "rigidbody/Sphere.h"

namespace alcube::physics {
  class SoftBodyParticle : public rigidbody::Sphere {
    public:
      explicit SoftBodyParticle();
      unsigned short index;
      SoftBodyParticle* alterEgo;
      float radiusForAlterEgo;
  };
}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
