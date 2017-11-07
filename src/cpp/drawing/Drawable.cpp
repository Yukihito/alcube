#include "Drawable.h"

namespace alcube::drawing {
  void Drawable::draw(Context& context) {
    glm::mat4 m = getModelMat();
    glm::mat4 mvp = context.vp * m;
    context.mvp = mvp;
    context.m = m;
    shader->shade(context);
    shape->draw();
  }
}
