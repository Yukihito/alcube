#ifndef ALCUBE_ACTOR_H
#define ALCUBE_ACTOR_H

#include <map>
#include "../physics/Features.h"
#include "../physics/Accessors.h"
#include "../../physics/Actor.h"
#include "../../physics/Simulator.h"
#include "../../utils/opencl/conversions.h"

namespace alcube::models::actor {
  class Actor : public physics::Accessors {
    public:
      glm::vec3 getPosition() override;
      void setPosition(glm::vec3 arg) override;
      glm::quat getRotation() override;
      void setRotation(glm::quat arg) override;
      glm::vec3 getLinearMomentum() override;
      void setLinearMomentum(glm::vec3 arg) override;
      glm::vec3 getAngularMomentum() override;
      void setAngularMomentum(glm::vec3 arg) override;

      explicit Actor(alcube::physics::Actor* physicsActor);
      alcube::physics::Actor* getPhysicsActor();
    private:
      alcube::physics::Actor* physicsActor;
  };

  class ActorFactory {
    public:
      Actor* create(physics::Features* feature);
  };
}

#endif //ALCUBE_ACTOR_H
