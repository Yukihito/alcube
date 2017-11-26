#ifndef ALCUBE_SHADERS_H
#define ALCUBE_SHADERS_H

#include "SimpleShader.h"
#include "StandardShader.h"
#include "ParticleShader.h"

namespace alcube::drawing::shaders {
  class Shaders {
    public:
      SimpleShader simple;
      StandardShader standard;
      ParticleShader particle;
      explicit Shaders(utils::FileUtil* fileUtil);
  };
}

#endif //ALCUBE_SHADERS_H
