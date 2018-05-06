#ifndef ALCUBE_DRAWABLE_H
#define ALCUBE_DRAWABLE_H

#include <glm/mat4x4.hpp>
#include "Shape.h"
#include "Shader.h"
#include "Context.h"
#include "Texture.h"

namespace alcube::drawing {
  class Drawable {
    public:
      void draw(Context& context);
      Shape* shape = nullptr;
      Shader* shader = nullptr;
      Texture* texture = nullptr;
      Material material;
  };
}

#endif //ALCUBE_DRAWABLE_H
