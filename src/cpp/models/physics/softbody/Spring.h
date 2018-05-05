#ifndef ALCUBE_MODELS_SPRING_H
#define ALCUBE_MODELS_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../../Actor.h"
#include "../../../physics/softbody/Spring.h"

namespace alcube::models::physics::softbody {
  class Spring {
    public:
      void init(alcube::physics::softbody::Spring* underlying);
      alcube::physics::softbody::Spring* getUnderlying();
    private:
      alcube::physics::softbody::Spring* underlying;
  };

  class SpringFactory {
    public:
      explicit SpringFactory(utils::MemoryPool<Spring>* memoryPool, alcube::physics::ActorFactory* actorFactory);
      float getK();
      void setK(float k);
      Spring* create(
        models::Actor* actor0, glm::vec3 position0,
        models::Actor* actor1, glm::vec3 position1
      );

    private:
      float k;
      utils::MemoryPool<Spring>* memoryPool;
      alcube::physics::ActorFactory* actorFactory;
  };
}

#endif //ALCUBE_MODELS_SPRING_H
