#include "ColorBalls.h"

namespace alcube::physics::samples::balls {
  ColorBalls::ColorBalls(
    drawing::shaders::Shaders *shaders,
    glm::vec3 diffuse,
    unsigned int maxModelCount,
    gpu::memories::Float3Memory* hostModelVerticesMemory,
    gpu::memories::Float3Memory* modelVerticesMemory,
    gpu::memories::Float3Memory* verticesMemory
  ) {
    shape = new drawing::shapes::triangles::Spheres(
      8,
      maxModelCount,
      hostModelVerticesMemory,
      modelVerticesMemory,
      verticesMemory
    );
    shader = &shaders->standard;
    material = new drawing::Material();
    material->diffuse = diffuse;
    material->ambient = diffuse / 2.0f;
    material->specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }

  glm::mat4 ColorBalls::getModelMat() {
    return glm::mat4();
  }

  drawing::Material* ColorBalls::getMaterial() {
    return this->material;
  }
}