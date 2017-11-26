#ifndef ALCUBE_PARTICLESHADER_H
#define ALCUBE_PARTICLESHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class ParticleShader : public Shader {
    public:
      explicit ParticleShader(utils::FileUtil* fileUtil);
      void shade(Context& context) override;
      void setupBuffer(Buffer* buffer) override;
  };
}

#endif //ALCUBE_PARTICLESHADER_H
