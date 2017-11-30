#include "Drawable.h"

namespace alcube::drawing {
  void Drawable::draw(Context& context) {
    glm::mat4 m = getModelMat();
    glm::mat4 mvp = context.vp * m;
    context.mvp = mvp;
    context.m = m;
    context.material = getMaterial();
    shader->shade(context);
    shape->draw();
  }

  Material* Drawable::getMaterial() {
    return nullptr;
  }
}
