#include "Camera.h"

namespace alcube::drawing {
  Camera::Camera(
    glm::vec3 position,
    glm::quat rotation,
    float angleOfView,
    float width,
    float height,
    float near,
    float far
  ) {
    this->position = position;
    this->rotation = rotation;
    this->angleOfView = angleOfView;
    this->aspectRatio = width / height;
    this->near = near;
    this->far = far;
  }
}

