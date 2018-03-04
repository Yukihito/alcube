#ifndef ALCUBE_DIRECTIONALLIGHTSHADER_H
#define ALCUBE_DIRECTIONALLIGHTSHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class DirectionalLightShader : public Shader {
    public:
      explicit DirectionalLightShader(utils::FileUtil* fileUtil);
      void shade(Context& context) override;
      void bindShape(Shape* shape) override;
      void unbindShape(Shape* shape) override;
      //void bindBuffer(Buffer *buffer) override;

  };
}

#endif //ALCUBE_DIRECTIONALLIGHTSHADER_H
