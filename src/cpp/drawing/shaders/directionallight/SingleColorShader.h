#ifndef ALCUBE_DIRECTIONALLIGHTSHADER_H
#define ALCUBE_DIRECTIONALLIGHTSHADER_H

#include "../../Shader.h"

namespace alcube::drawing::shaders::directionallight {
  class SingleColorShader : public Shader {
    public:
      explicit SingleColorShader(utils::FileUtil* fileUtil, Context& context);
  };
}

#endif //ALCUBE_DIRECTIONALLIGHTSHADER_H
