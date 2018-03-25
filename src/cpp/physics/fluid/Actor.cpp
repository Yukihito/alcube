#include "Actor.h"

namespace alcube::physics::fluid {
  Actor::Actor() : physics::Actor() {
    this->actor.type = FLUID;
    this->subPhysicalQuantity = {};
  }
}