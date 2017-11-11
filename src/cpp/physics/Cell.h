#ifndef ALCUBE_PHYSICS_CELL_H
#define ALCUBE_PHYSICS_CELL_H

#include "rigidbody/Sphere.h"
#include "rigidbody/Spring.h"

namespace alcube::physics {
  class Cell : public rigidbody::Sphere, rigidbody::SpringsHolder {
    public:
      explicit Cell();
      glm::vec3 getPosition() override;
      glm::quat getRotation() override;
  };
}

#endif //ALCUBE_PHYSICS_CELL_H
