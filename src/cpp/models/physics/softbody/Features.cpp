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

  void Features::init(int id) {
    physics::Features::init(id);
    elasticity = 1.0f;
    mass = 1.0f;
  }

  FeaturesFactory::FeaturesFactory(
    alcube::utils::MemoryPool<alcube::models::physics::softbody::Features> *memoryPool) {
    this->memoryPool = memoryPool;
  }

  Features* FeaturesFactory::create() {
    return memoryPool->get();
  }
}
