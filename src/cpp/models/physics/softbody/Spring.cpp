#include "Spring.h"

namespace alcube::models::physics::softbody {
  void Spring::init(alcube::physics::softbody::Spring *underlying) {
    this->underlying = underlying;
  }

  alcube::physics::softbody::Spring* Spring::getUnderlying() {
    return underlying;
  }

  void SpringFactory::setK(float k) {
    this->k = k;
  }

  float SpringFactory::getK() {
    return k;
  }

  SpringFactory::SpringFactory(
    alcube::utils::MemoryPool<alcube::models::physics::softbody::Spring>* memoryPool,
    alcube::physics::ActorFactory* actorFactory
  ) {
    this->memoryPool = memoryPool;
    this->actorFactory = actorFactory;
  }

  Spring* SpringFactory::create(
    models::Actor *actor0, glm::vec3 position0,
    models::Actor *actor1, glm::vec3 position1
  ) {
    auto underlying = actorFactory->createSpring();
    underlying->setK(k);
    underlying->getNode(0)->setActor((alcube::physics::softbody::Actor*)actor0->getPhysicsActor());
    underlying->getNode(0)->setPosition(position0);
    underlying->getNode(1)->setActor((alcube::physics::softbody::Actor*)actor1->getPhysicsActor());
    underlying->getNode(1)->setPosition(position1);
    auto spring = memoryPool->get();
    spring->init(underlying);
    return spring;
  }
}
