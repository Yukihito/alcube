#include "SphereDrawable.h"

namespace alcube::samples {
  SphereDrawable::SphereDrawable(
    drawing::Shader &shader,
    glm::vec3 diffuse,
    unsigned int maxInstanceCount,
    GLfloat* positions,
    GLfloat* rotations0,
    GLfloat* rotations1,
    GLfloat* rotations2,
    GLfloat* rotations3,
    GLfloat* colors
  ) {
    shape = new drawing::shapes::triangles::Sphere(
      8,
      maxInstanceCount,
      positions,
      rotations0,
      rotations1,
      rotations2,
      rotations3,
      colors
    );
    this->shader = &shader;
    material.diffuse = diffuse;
    material.ambient = diffuse / 2.0f;
    material.specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }
}
