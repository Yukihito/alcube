#ifndef ALCUBE_DIRECTIONALLIGHTSHADER_H
#define ALCUBE_DIRECTIONALLIGHTSHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class DirectionalLightShader : public Shader {
    public:
      explicit DirectionalLightShader(utils::FileUtil* fileUtil);
      void shade(Context& context) override;
      //void initializeBuffer(Buffer* buffer) override;
      void bindBuffer(Buffer *buffer) override;
      void unbindBuffer() override;
  };
}

#endif //ALCUBE_DIRECTIONALLIGHTSHADER_H
