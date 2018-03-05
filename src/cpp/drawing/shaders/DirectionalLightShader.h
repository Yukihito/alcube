#ifndef ALCUBE_DIRECTIONALLIGHTSHADER_H
#define ALCUBE_DIRECTIONALLIGHTSHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class DirectionalLightShader : public Shader {
    public:
      explicit DirectionalLightShader(utils::FileUtil* fileUtil);
      void bindUniforms(Context &context) override;
  };
}

#endif //ALCUBE_DIRECTIONALLIGHTSHADER_H
