#ifndef ALCUBE_SAMPLES_SPHEREDRAWABLES_H
#define ALCUBE_SAMPLES_SPHEREDRAWABLES_H

#include <glm/vec3.hpp>
#include "../../gpu/GPUAccessor.h"
#include "../../drawing/shaders/Shaders.h"
#include "../../drawing/Drawable.h"
#include "../../drawing/shapes/triangles/Sphere.h"

namespace alcube::models::drawing {
  class SphereDrawable : public alcube::drawing::Drawable {
    public:
      SphereDrawable(
        alcube::drawing::Shader &shader,
        alcube::drawing::Material material,
        unsigned int maxInstanceCount,
        GLfloat* positions,
        GLfloat* rotations0,
        GLfloat* rotations1,
        GLfloat* rotations2,
        GLfloat* rotations3,
        GLfloat* colors
      );
  };
}

#endif //ALCUBE_SPHEREDRAWABLES_H
