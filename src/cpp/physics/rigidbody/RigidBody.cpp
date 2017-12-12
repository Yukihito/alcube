#include "RigidBody.h"

namespace alcube::physics::rigidbody {
  RigidBody::RigidBody() {
    this->mass = 1.0f;
    this->elasticity = 1.0f;
    this->staticFrictionCoefficient = 0.0f;
    this->dynamicFrictionCoefficient = 0.0f;
    this->position = glm::vec3();
    this->rotation = glm::quat();
    this->linearMomentum = glm::vec3();
    this->angularMomentum = glm::vec3();
  }
}
