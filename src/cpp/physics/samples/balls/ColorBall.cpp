#include "ColorBall.h"

namespace alcube::physics::samples::balls {
  ColorBall::ColorBall(
    drawing::shapes::Shapes *shapes,
    drawing::shaders::Shaders *shaders,
    glm::vec3 diffuse,
    bool showsLinearMomentum
  ) {
    shape = &shapes->triangles.sphere;
    shader = &shaders->standard;
    material = new drawing::Material();
    material->diffuse = diffuse;
    material->ambient = diffuse / 2.0f;
    material->specular = glm::vec3(0.3f, 0.3f, 0.3f);
    this->showsLinearMomentum = showsLinearMomentum;
  }

  glm::mat4 ColorBall::getModelMat() {
    return glm::translate(position) * glm::toMat4(rotation) * glm::scale(glm::vec3(radius, radius, radius));
  }

  drawing::Material *ColorBall::getMaterial() {
    if (showsLinearMomentum) {
      glm::vec3 diffuse = glm::vec3(
        (sqrtf(linearMomentum.x * linearMomentum.x) / 15.0f),
        (sqrtf(linearMomentum.y * linearMomentum.y) / 15.0f),
        (sqrtf(linearMomentum.z * linearMomentum.z) / 15.0f));
      material->diffuse = diffuse;
      material->ambient = diffuse / 2.0f;
    }
    return this->material;
  }
}