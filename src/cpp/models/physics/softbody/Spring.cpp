#include "Spring.h"

namespace alcube::models::physics::softbody {
  Spring::Spring(alcube::physics::softbody::Spring *underlying) {
    this->underlying = underlying;
  }

  alcube::physics::softbody::Spring* Spring::getUnderlying() {
    return underlying;
  }

  void SpringFactory::setK(float k) {
    this->k = k;
  }

  Spring* SpringFactory::create(
    models::Actor *actor0, glm::vec3 position0,
    models::Actor *actor1, glm::vec3 position1) {
    auto spring = new alcube::physics::softbody::Spring();
    spring->nodes[0].actor = actor0->getPhysicsActor();
    spring->nodes[0].position = position0;
    spring->nodes[1].actor = actor1->getPhysicsActor();
    spring->nodes[1].position = position1;
    spring->k = k;
    return new Spring(spring);
  }
}
