#ifndef ALCUBE_DIRECTIONALLIGHT_INSTANCECOLORSHADER_H
#define ALCUBE_DIRECTIONALLIGHT_INSTANCECOLORSHADER_H

#include "../../Shader.h"

namespace alcube::drawing::shaders::directionallight {
  class InstanceColorShader : public Shader {
    public:
      explicit InstanceColorShader(utils::FileUtil* fileUtil, Context& context);
  };
}


#endif //ALCUBE_DIRECTIONALLIGHT_INSTANCECOLORSHADER_H
