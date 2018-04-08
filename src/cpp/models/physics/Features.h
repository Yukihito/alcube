#ifndef ALCUBE_PHYSICSFEATURE_H
#define ALCUBE_PHYSICSFEATURE_H

#include "../../physics/Actor.h"

namespace alcube::models::physics {
  class Features {
    public:
      int getId();
      virtual alcube::physics::Actor* createPhysicsActor() = 0;
      virtual void init(int id);

    private:
      int id = 0;
  };
}

#endif //ALCUBE_PHYSICSFEATURE_H
