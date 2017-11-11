#ifndef ALCUBE_SPHERE_H
#define ALCUBE_SPHERE_H

#include "RigidBody.h"

namespace alcube::physics::rigidbody {
  class Sphere : public RigidBody {
    public:
      float radius;
      explicit Sphere();
  };
}


#endif //ALCUBE_SPHERE_H
