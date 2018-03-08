#ifndef ALCUBE_CONTEXT_H
#define ALCUBE_CONTEXT_H

#include <glm/mat4x4.hpp>
#include "Material.h"

namespace alcube::drawing {
  class Context {
    public:
      glm::mat4 vp;
      glm::mat4 v;
      Material material;
  };
}

#endif //ALCUBE_CONTEXT_H
