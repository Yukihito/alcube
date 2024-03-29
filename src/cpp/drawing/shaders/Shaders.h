#ifndef ALCUBE_SHADERS_H
#define ALCUBE_SHADERS_H

#include "ParticleShader.h"
#include "directionallight/SingleColorShader.h"
#include "directionallight/InstanceColorShader.h"
#include "directionallight/TextureSingleColorShader.h"
#include "directionallight/TextureInstanceColorShader.h"

namespace alcube::drawing::shaders {
  struct DirectionalLight {
    directionallight::SingleColorShader singleColor;
    directionallight::InstanceColorShader instanceColor;
    directionallight::TextureSingleColorShader textureSingleColor;
    directionallight::TextureInstanceColorShader textureInstanceColorShader;
    explicit DirectionalLight(utils::FileUtil* fileUtil, Context& context);
  };
  class Shaders {
    public:
      DirectionalLight directionalLight;
      explicit Shaders(utils::FileUtil* fileUtil, Context& context);
  };
}

#endif //ALCUBE_SHADERS_H
