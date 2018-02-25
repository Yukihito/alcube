#ifndef ALCUBE_DRAWABLE_H
#define ALCUBE_DRAWABLE_H

#include <glm/mat4x4.hpp>
#include "Shape.h"
#include "Shader.h"
#include "Context.h"

namespace alcube::drawing {
  class DrawableBuffer {
    public:
      glm::mat4 matrix;
      Material material;
  };
  class Drawable {
    public:
      void draw(Context& context);
      Shape* shape = nullptr;
      Shader* shader = nullptr;
      DrawableBuffer buffer[2];
      int* bufferIndex = nullptr;
      void updateBuffer();
      bool isGroup = false;
    protected:
      virtual glm::mat4 getModelMat() = 0;
      virtual Material* getMaterial();
  };
}

#endif //ALCUBE_DRAWABLE_H
