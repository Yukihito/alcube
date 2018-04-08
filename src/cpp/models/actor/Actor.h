#ifndef ALCUBE_ACTOR_H
#define ALCUBE_ACTOR_H

#include <map>
#include <stack>
#include "../physics/Features.h"
#include "../physics/Accessors.h"
#include "../../physics/Actor.h"
#include "../../physics/Simulator.h"
#include "../../utils/opencl/conversions.h"
#include "../../utils/MemoryPool.h"

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
      void init(alcube::physics::Actor* physicsActor);
      void init(int id);

      alcube::physics::Actor* getPhysicsActor();
    private:
      alcube::physics::Actor* physicsActor = nullptr;
      int id = 0;
  };

  class ActorFactory {
    public:
      explicit ActorFactory(utils::MemoryPool<Actor>* memoryPool);
      Actor* create(physics::Features* feature);

    private:
      utils::MemoryPool<Actor>* memoryPool;
  };
}

#endif //ALCUBE_ACTOR_H
