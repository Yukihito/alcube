#ifndef ALCUBE_SIMPLESHADER_H
#define ALCUBE_SIMPLESHADER_H

#include "../Shader.h"

namespace drawing::shaders {
  class SimpleShader : public Shader {
    public:
      explicit SimpleShader(utils::FileUtil* fileUtil);
      void shade(Context& context) override;
      void setupBuffer(Buffer* buffer) override;
  };
}

#endif //ALCUBE_SIMPLESHADER_H
