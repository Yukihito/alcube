#include "Drawable.h"

namespace alcube::drawing {
  void Drawable::draw(Context& context) {
    glm::mat4 matrix = getModelMat();
    glm::mat4 mvp = context.vp * matrix;
    context.mvp = mvp;
    context.m = matrix;
    context.mv = context.v * matrix;
    context.material = *getMaterial();
    shader->bindUniforms();
    shape->draw();
  }

  Material* Drawable::getMaterial() {
    return nullptr;
  }
}
