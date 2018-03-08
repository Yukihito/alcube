#include "DrawableFluidParticle.h"

namespace alcube::physics::samples {
  DrawableFluidParticle::DrawableFluidParticle(
    drawing::shapes::Shapes *shapes,
    drawing::shaders::Shaders *shaders,
    glm::vec3 diffuse
  ) {
    shape = &shapes->triangles.sphere;
    shader = & shaders->directionalLight;
    material.diffuse = diffuse;
    material.ambient = diffuse / 2.0f;
    material.specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }

  /*
  glm::mat4 DrawableFluidParticle::getModelMat() {
    return glm::translate(position);
  }

  drawing::Material *DrawableFluidParticle::getMaterial() {
    return this->material;
  }
   */
}
