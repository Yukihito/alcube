#ifndef ALCUBE_PHYSICSFEATURE_H
#define ALCUBE_PHYSICSFEATURE_H

#include "../../physics/Actor.h"
#include "../../physics/ActorFactory.h"
#include <vector>

namespace alcube::models::physics {
  class Features {
    public:
      int getId();
      virtual alcube::physics::Actor* createPhysicsActor() = 0;
      virtual void init(int id, alcube::physics::ActorFactory* actorFactory);

    protected:
      alcube::physics::ActorFactory* actorFactory;
    private:
      int id = 0;
  };
}

#endif //ALCUBE_PHYSICSFEATURE_H
