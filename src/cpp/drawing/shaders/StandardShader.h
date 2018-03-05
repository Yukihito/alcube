#ifndef ALCUBE_STANDARDSHADER_H
#define ALCUBE_STANDARDSHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class StandardShader : public Shader {
    public:
      explicit StandardShader(utils::FileUtil* fileUtil);
      //void bindUniforms(Context &context) override;
      //void bindBuffer(Buffer *buffer) override;
      bool initialized = false;
  };
}

#endif //ALCUBE_STANDARDSHADER_H
