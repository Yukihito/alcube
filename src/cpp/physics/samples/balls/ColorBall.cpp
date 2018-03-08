#include "ColorBall.h"

namespace alcube::physics::samples::balls {
  ColorBall::ColorBall(
    drawing::shapes::Shapes *shapes,
    drawing::shaders::Shaders *shaders,
    glm::vec3 diffuse
  ) {
    shape = &shapes->triangles.sphere;
    shader = &shaders->directionalLight;
    material = new drawing::Material();
    material->diffuse = diffuse;
    material->ambient = diffuse / 2.0f;
    material->specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }
}