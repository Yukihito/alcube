#ifndef ALCUBE_SIMPLESHADER_H
#define ALCUBE_SIMPLESHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class SimpleShader : public Shader {
    public:
      explicit SimpleShader(utils::FileUtil* fileUtil);
      //void bindUniforms(Context &context) override;
      //void bindBuffer(Buffer *buffer) override;
  };
}

#endif //ALCUBE_SIMPLESHADER_H
