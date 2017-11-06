#ifndef ALCUBE_STANDARDSHADER_H
#define ALCUBE_STANDARDSHADER_H

#include "../Shader.h"

namespace drawing::shaders {
  class StandardShader : public Shader {
    public:
      explicit StandardShader(utils::FileUtil* fileUtil);
      void shade(Context& context) override;
      void setupBuffer(Buffer* buffer) override;
  };
}

#endif //ALCUBE_STANDARDSHADER_H
