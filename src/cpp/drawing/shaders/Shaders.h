#ifndef ALCUBE_SHADERS_H
#define ALCUBE_SHADERS_H

#include "SimpleShader.h"
#include "StandardShader.h"
#include "ParticleShader.h"
#include "TileShader.h"
#include "directionallight/SingleColorShader.h"
#include "directionallight/InstanceColorShader.h"
#include "directionallight/TextureSingleColorShader.h"

namespace alcube::drawing::shaders {
  struct DirectionalLight {
    directionallight::SingleColorShader singleColor;
    directionallight::InstanceColorShader instanceColor;
    directionallight::TextureSingleColorShader textureSingleColor;
    explicit DirectionalLight(utils::FileUtil* fileUtil, Context& context);
  };
  class Shaders {
    public:
      //SimpleShader simple;
      //StandardShader standard;
      //ParticleShader particle;
      //TileShader tile;
      DirectionalLight directionalLight;
      explicit Shaders(utils::FileUtil* fileUtil, Context& context);
  };
}

#endif //ALCUBE_SHADERS_H
