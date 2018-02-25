#include "Drawable.h"

namespace alcube::drawing {
  void Drawable::draw(Context& context) {
    int drawableBufferIndex = bufferIndex[0];
    glm::mat4 mvp = context.vp * buffer[drawableBufferIndex].matrix;
    context.mvp = mvp;
    context.m = buffer[bufferIndex[0]].matrix;
    context.material = buffer[drawableBufferIndex].material;
    shader->shade(context);
    shape->draw();
  }

  Material* Drawable::getMaterial() {
    return nullptr;
  }

  void Drawable::updateBuffer() {
    int freeBufferIndex = bufferIndex[0] == 0 ? 1 : 0;
    buffer[freeBufferIndex].matrix = getModelMat();
    Material* material = getMaterial();
    if (material != nullptr) {
      buffer[freeBufferIndex].material = *material;
    }
  }
}
