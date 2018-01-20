#ifndef ALCUBE_PHYSICS_CELL_H
#define ALCUBE_PHYSICS_CELL_H

#include "rigidbody/Sphere.h"

namespace alcube::physics {
  class Cell : public rigidbody::Sphere {
    public:
      explicit Cell();
      unsigned short index;
      Cell* alterEgo;
      float radiusForAlterEgo;
  };
}

#endif //ALCUBE_PHYSICS_CELL_H
