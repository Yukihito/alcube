#ifndef ALCUBE_SPRING_H
#define ALCUBE_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Actor.h"

namespace alcube::physics::softbody {
  class SpringNode {
    public:
      Actor* particle;
      glm::vec3 position;
  };

  class Spring {
    public:
      SpringNode nodes[2];
      float k;
  };
}

#endif //ALCUBE_SPRING_H
