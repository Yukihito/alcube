#include "Features.h"

namespace alcube::models::physics::softbody {
  float Features::getMass() {
    return mass;
  }

  void Features::setMass(float arg) {
    mass = arg;
  }

  float Features::getElasticity() {
    return elasticity;
  }

  void Features::setElasticity(float arg) {
    elasticity = arg;
  }

  alcube::physics::Actor* Features::createPhysicsActor() {
    auto actor = new alcube::physics::softbody::Actor();
    actor->subPhysicalQuantity.elasticity = elasticity;
    actor->physicalQuantity.mass = mass;
    return actor;
  }
}