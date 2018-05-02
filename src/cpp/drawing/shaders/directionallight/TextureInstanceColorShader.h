#ifndef ALCUBE_TEXTUREINSTANCECOLORSHADER_H
#define ALCUBE_TEXTUREINSTANCECOLORSHADER_H

#include "../../Shader.h"

namespace alcube::drawing::shaders::directionallight {
  class TextureInstanceColorShader : public Shader {
    public:
      explicit TextureInstanceColorShader(utils::FileUtil* fileUtil, Context& context);
  };
}



#endif //ALCUBE_TEXTUREINSTANCECOLORSHADER_H
