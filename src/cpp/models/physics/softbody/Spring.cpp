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

  SpringFactory::SpringFactory(alcube::utils::MemoryPool<alcube::models::physics::softbody::Spring>* memoryPool) {
    this->memoryPool = memoryPool;
  }

  Spring* SpringFactory::create(
    models::Actor *actor0, glm::vec3 position0,
    models::Actor *actor1, glm::vec3 position1
  ) {
    auto underlying = new alcube::physics::softbody::Spring();
    underlying->nodes[0].actor = actor0->getPhysicsActor();
    underlying->nodes[0].position = position0;
    underlying->nodes[1].actor = actor1->getPhysicsActor();
    underlying->nodes[1].position = position1;
    underlying->k = k;
    auto spring = memoryPool->get();
    spring->init(underlying);
    return spring;
  }
}
