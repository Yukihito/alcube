#include "Actor.h"

namespace alcube::physics::fluid {
  Actor::Actor() : physics::Actor() {
    this->actor.type = FLUID;
    this->subPhysicalQuantity = {};
    Actor::instances[this]= this;
  }

  std::map<physics::Actor*, Actor*> Actor::instances = {};
  float Actor::stiffness = 64.0f;
  float Actor::density = 0.02f;
  float Actor::viscosity = 8.0f;
  float Actor::mass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * Actor::density;
}