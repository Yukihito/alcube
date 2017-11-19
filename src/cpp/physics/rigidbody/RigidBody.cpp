#include "RigidBody.h"

namespace alcube::physics::rigidbody {
  RigidBody::RigidBody() {
    this->mass = 1.0f;
    this->currentState.position = glm::vec3();
    this->currentState.rotation = glm::quat();
    this->currentState.linearMomentum = glm::vec3();
    this->currentState.angularMomentum = glm::vec3();
  }
}
