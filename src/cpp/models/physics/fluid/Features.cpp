#include "Features.h"

namespace alcube::models::physics::fluid {
  float Features::getMass() {
    return alcube::physics::fluid::Actor::mass;
  }

  float Features::getDensity() {
    return alcube::physics::fluid::Actor::density;
  }

  void Features::setDensity(float arg) {
    alcube::physics::fluid::Actor::density = arg;
    alcube::physics::fluid::Actor::mass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * alcube::physics::fluid::Actor::density;
  }

  float Features::getStiffness() {
    return alcube::physics::fluid::Actor::stiffness;
  }

  void Features::setStiffness(float arg) {
    alcube::physics::fluid::Actor::stiffness = arg;
  }

  float Features::getViscosity() {
    return alcube::physics::fluid::Actor::viscosity;
  }

  void Features::setViscosity(float arg) {
    alcube::physics::fluid::Actor::viscosity = arg;
  }

  alcube::physics::Actor* Features::createPhysicsActor() {
    auto actor = actorFactory->createFluid();
    actor->getDto()->mass = alcube::physics::fluid::Actor::mass;
    return actor;
  }

  FeaturesFactory::FeaturesFactory(
    utils::MemoryPool<Features> *memoryPool,
    alcube::physics::ActorFactory* actorFactory
  ) {
    this->memoryPool = memoryPool;
    this->actorFactory = actorFactory;
  }

  Features* FeaturesFactory::create() {
    auto features = memoryPool->allocate();
    int nextId = instanceCount;
    instanceCount++;
    features->init(nextId, actorFactory);
    return features;
  }
}
