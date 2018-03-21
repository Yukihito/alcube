#include "Actor.h"

namespace alcube::physics {
  Actor::Actor() {
    this->index = 0;
    this->actor = {};
    this->physicalQuantity = {};
    this->physicalQuantity.radius = 1.0f;
    this->physicalQuantity.mass = 1.0f;
    this->physicalQuantity.rotation = {0.0f, 0.0f, 0.0f, 1.0f};
    this->physicalQuantity.angularMomentum = {0.0f, 0.0f, 0.0f};
    this->physicalQuantity.linearMomentum = {0.0f, 0.0f, 0.0f};
    this->physicalQuantity.position = {0.0f, 0.0f, 0.0f};
  }
}