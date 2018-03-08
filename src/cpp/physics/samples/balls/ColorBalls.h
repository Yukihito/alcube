#ifndef ALCUBE_COLORBALLS_H
#define ALCUBE_COLORBALLS_H

#include "../../../drawing/Material.h"
#include "../../../drawing/shaders/Shaders.h"
#include "../../../drawing/Drawable.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include "../../../drawing/shapes/triangles/Sphere.h"
#include "../../../gpu/GPU.h"

namespace alcube::physics::samples::balls {
  class ColorBalls : public drawing::Drawable {
    public:
      ColorBalls(
        drawing::shaders::Shaders *shaders,
        glm::vec3 diffuse,
        unsigned int maxInstanceCount,
        gpu::memories::Float3Memory* positionsMemory
      );
  };
}

#endif //ALCUBE_COLORBALLS_H
