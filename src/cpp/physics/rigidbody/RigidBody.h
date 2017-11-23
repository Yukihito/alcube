#ifndef ALCUBE_RIGIDBODY_H
#define ALCUBE_RIGIDBODY_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace alcube::physics::rigidbody {
  class RigidBodyState {
    public:
      glm::vec3 position;
      glm::quat rotation;
      glm::vec3 linearMomentum;
      glm::vec3 angularMomentum;
  };

  class RigidBody {
    public:
      RigidBodyState currentState;
      RigidBodyState nextState;
      float mass;
      float elasticity;
      explicit RigidBody();
  };
}


#endif //ALCUBE_RIGIDBODY_H
