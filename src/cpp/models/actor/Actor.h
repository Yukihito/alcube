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
      int getId();
      glm::vec3 getPosition() override;
      void setPosition(glm::vec3 arg) override;
      glm::quat getRotation() override;
      void setRotation(glm::quat arg) override;
      glm::vec3 getLinearMomentum() override;
      void setLinearMomentum(glm::vec3 arg) override;
      glm::vec3 getAngularMomentum() override;
      void setAngularMomentum(glm::vec3 arg) override;
      // package private:
      explicit Actor(int id, int physicsFeaturesId, alcube::physics::Actor* physicsActor);
      int getPhysicsFeaturesId();
      virtual void addTo(alcube::physics::Simulator* physicsSimulator) = 0;

    private:
      int id;
      int physicsFeaturesId;
      alcube::physics::Actor* physicsActor;
  };

  class ActorRepository {
    public:
      Actor* find(int id);
      void store(Actor* entity);
      void del(int id);

    private:
      std::map<int, Actor*> actors;
  };

  class ActorFactory {
    public:
      virtual Actor* create(physics::Features* feature) = 0;
  };
}

#endif //ALCUBE_ACTOR_H
