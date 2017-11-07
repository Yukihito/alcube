#ifndef ALCUBE_CAMERA_H
#define ALCUBE_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace alcube::drawing {
  class Camera {
    public:
      glm::vec3 position;
      glm::quat rotation;
      float near;
      float far;
      float aspectRatio;
      float angleOfView;
      explicit Camera(
        glm::vec3 position,
        glm::quat rotation,
        float angleOfView,
        float width,
        float height,
        float near,
        float far
      );
  };
}


#endif //ALCUBE_CAMERA_H
