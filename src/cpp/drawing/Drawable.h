#ifndef ALCUBE_DRAWABLE_H
#define ALCUBE_DRAWABLE_H

#include <glm/mat4x4.hpp>
#include "Shape.h"
#include "Shader.h"
#include "Context.h"

namespace drawing {
  class Drawable {
    public:
      void draw(Context& context);
      Shape* shape = nullptr;
      Shader* shader = nullptr;
    protected:
      virtual glm::mat4 getModelMat() = 0;
  };
}

#endif //ALCUBE_DRAWABLE_H
