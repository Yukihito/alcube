#ifndef ALCUBE_TEXTURESINGLECOLORSHADER_H
#define ALCUBE_TEXTURESINGLECOLORSHADER_H

#include "../../Shader.h"

namespace alcube::drawing::shaders::directionallight {
  class TextureSingleColorShader : public Shader {
    public:
      explicit TextureSingleColorShader(utils::FileUtil* fileUtil, Context& context);
  };
}

#endif //ALCUBE_TEXTURESINGLECOLORSHADER_H
