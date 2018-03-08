#include "Drawable.h"

namespace alcube::drawing {
  void Drawable::draw(Context& context) {
    shape->update();
    context.material = material;
    shader->bindUniforms();
    shader->bindShape(shape);
    shape->draw();
    shader->unbindShape(shape);
  }
}
