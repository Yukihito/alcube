#ifndef ALCUBE_SPRING_H
#define ALCUBE_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "SoftBodyActor.h"

namespace alcube::physics {
  class SpringNode {
    public:
      SoftBodyActor* particle;
      glm::vec3 position;
  };

  class Spring {
    public:
      SpringNode nodes[2];
      float k;
  };
}

#endif //ALCUBE_SPRING_H
