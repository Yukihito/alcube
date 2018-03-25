#ifndef ALCUBE_MODELS_SPRING_H
#define ALCUBE_MODELS_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../../actor/Actor.h"

namespace alcube::models::physics::softbody {
  class Spring {
    public:
      explicit Spring(alcube::physics::softbody::Spring* underlying);
      alcube::physics::softbody::Spring* getUnderlying();
    private:
      alcube::physics::softbody::Spring* underlying;
  };

  class SpringFactory {
    public:
      void setK(float k);
      Spring* create(
        models::actor::Actor* actor0, glm::vec3 position0,
        models::actor::Actor* actor1, glm::vec3 position1
      );

    private:
      float k;
  };
}

#endif //ALCUBE_MODELS_SPRING_H
