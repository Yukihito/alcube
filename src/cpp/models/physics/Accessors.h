#ifndef ALCUBE_PHYSICSACCESSORS_H
#define ALCUBE_PHYSICSACCESSORS_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace alcube::models::physics {
  class Accessors {
    public:
      virtual glm::vec3 getPosition() = 0;
      virtual void setPosition(glm::vec3 arg) = 0;
      virtual glm::quat getRotation() = 0;
      virtual void setRotation(glm::quat arg) = 0;
      virtual glm::vec3 getLinearMomentum() = 0;
      virtual void setLinearMomentum(glm::vec3 arg) = 0;
      virtual glm::vec3 getAngularMomentum() = 0;
      virtual void setAngularMomentum(glm::vec3 arg) = 0;
  };
}

#endif //ALCUBE_PHYSICSACCESSORS_H
