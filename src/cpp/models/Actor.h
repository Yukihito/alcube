#ifndef ALCUBE_ACTOR_H
#define ALCUBE_ACTOR_H

#include <map>
#include <stack>
#include "physics/Features.h"
#include "physics/Accessors.h"
#include "../physics/Actor.h"
#include "../physics/Simulator.h"
#include "../utils/opencl/conversions.h"
#include "../utils/MemoryPool.h"
#include "drawing/Renderer.h"
#include "v8.h"

namespace alcube::models {
  class Actor : public physics::Accessors, public drawing::IndexHolder {
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
      glm::vec3 getColor();
      void setColor(glm::vec3 arg);
      void init(
        int id,
        alcube::physics::Actor* physicsActor,
        alcube::models::drawing::InstanceRenderer* instanceRenderer
      );

      alcube::physics::Actor* getPhysicsActor();
      unsigned int getIndex() override;
    private:
      alcube::physics::Actor* physicsActor = nullptr;
      alcube::models::drawing::InstanceRenderer* instanceRenderer = nullptr;
      int id = 0;
  };

  class ActorFactory {
    public:
      explicit ActorFactory(
        utils::MemoryPool<Actor>* memoryPool,
        drawing::InstanceRendererFactory* instanceRendererFactory
      );
      Actor* create(
        physics::Features* feature,
        models::drawing::Renderer* renderer
      );

    private:
      int instanceCount = 0;
      utils::MemoryPool<Actor>* memoryPool;
      drawing::InstanceRendererFactory* instanceRendererFactory;
  };
}

#endif //ALCUBE_ACTOR_H
