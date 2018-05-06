#include "SphereDrawable.h"

namespace alcube::models::drawing {
  SphereDrawable::SphereDrawable(
    alcube::drawing::Shader &shader,
    unsigned int maxInstanceCount,
    GLfloat* positions,
    GLfloat* rotations0,
    GLfloat* rotations1,
    GLfloat* rotations2,
    GLfloat* rotations3,
    GLfloat* colors
  ) {
    shape = new alcube::drawing::shapes::triangles::Sphere(
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
    this->material.ambient = glm::vec3();
    this->material.specular = glm::vec3();
    this->material.diffuse = glm::vec3();
  }
}
