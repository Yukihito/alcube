#include "Actor.h"

namespace alcube::physics::softbody {
  Actor::Actor() : physics::Actor() {
    this->actor.type = SOFT_BODY;
    this->subPhysicalQuantity = {};
    this->subPhysicalQuantity.elasticity = 1.0f;
    Actor::instances[this]= this;
  }

  std::map<physics::Actor*, Actor*> Actor::instances = {};
}
