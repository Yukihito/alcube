#ifndef ALCUBE_SPRING_H
#define ALCUBE_SPRING_H

#include "Cell.h"

namespace alcube::physics {
  class SpringNode {
    public:
      Cell* cell;
      glm::vec3 position;
  };

  class Spring {
    public:
      SpringNode nodes[2];
      float k;
  };
}


#endif //ALCUBE_SPRING_H
