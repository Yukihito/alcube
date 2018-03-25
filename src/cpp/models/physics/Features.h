#ifndef ALCUBE_PHYSICSFEATURE_H
#define ALCUBE_PHYSICSFEATURE_H

#include "../../physics/Actor.h"
#include "../../physics/softbody/Spring.h"
#include "../../physics/fluid/Actor.h"

namespace alcube::models::physics {
  class Features {
    public:
      virtual alcube::physics::Actor* createPhysicsActor() = 0;
  };
}

#endif //ALCUBE_PHYSICSFEATURE_H
