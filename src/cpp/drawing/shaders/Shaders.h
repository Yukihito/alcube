#ifndef ALCUBE_SHADERS_H
#define ALCUBE_SHADERS_H

#include "SimpleShader.h"
#include "StandardShader.h"

namespace drawing::shaders {
  class Shaders {
    public:
      SimpleShader simple;
      StandardShader standard;
      explicit Shaders(utils::FileUtil* fileUtil);
  };
}

#endif //ALCUBE_SHADERS_H
