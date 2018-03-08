#include "ColorBalls.h"

namespace alcube::physics::samples::balls {
  ColorBalls::ColorBalls(
    drawing::shaders::Shaders *shaders,
    glm::vec3 diffuse,
    unsigned int maxInstanceCount,
    gpu::memories::Float3Memory* positionsMemory
  ) {
    shape = new drawing::shapes::triangles::MultiSphere(
      8,
      maxInstanceCount,
      (GLfloat*)positionsMemory->dto
    );
    shader = &shaders->directionalLight;
    material.diffuse = diffuse;
    material.ambient = diffuse / 2.0f;
    material.specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }

  /*
  glm::mat4 ColorBalls::getModelMat() {
    return glm::mat4();
  }

  drawing::Material* ColorBalls::getMaterial() {
    return this->material;
  }
   */
}