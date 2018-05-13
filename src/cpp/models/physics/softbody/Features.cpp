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
    auto actor = actorFactory->createSoftbody();
    actor->getSubPhysicalQuantity()->elasticity = elasticity;
    actor->getDto()->mass = mass;
    return actor;
  }

  void Features::init(int id, alcube::physics::ActorFactory* actorFactory) {
    physics::Features::init(id, actorFactory);
    elasticity = 1.0f;
    mass = 1.0f;
  }

  FeaturesFactory::FeaturesFactory(
    alcube::utils::MemoryPool<alcube::models::physics::softbody::Features> *memoryPool,
    alcube::physics::ActorFactory* actorFactory
  ) {
    this->memoryPool = memoryPool;
    this->actorFactory = actorFactory;
  }

  Features* FeaturesFactory::create() {
    auto features = memoryPool->get();
    int nextId = instanceCount;
    instanceCount++;
    features->init(nextId, actorFactory);
    return features;
  }
}
