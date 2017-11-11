#include "Cell.h"

namespace alcube::physics {
  Cell::Cell() : Sphere() {
    springs = {};
  }

  glm::vec3 Cell::getPosition() {
    return currentState.position;
  }

  glm::quat Cell::getRotation() {
    return currentState.rotation;
  }
}