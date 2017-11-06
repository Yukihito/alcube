#ifndef ALCUBE_SHAPE_H
#define ALCUBE_SHAPE_H

#include <glm/mat4x4.hpp>
#include "Buffer.h"
#include "Shader.h"

namespace drawing {
  class Shape {
    public:
      virtual void draw() = 0;
      Buffer* buffer = nullptr;

    protected:
      virtual Buffer* createBuffer() = 0;
      void initialize();
  };
}


#endif //ALCUBE_SHAPE_H
