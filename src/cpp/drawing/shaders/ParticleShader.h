#ifndef ALCUBE_PARTICLESHADER_H
#define ALCUBE_PARTICLESHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class ParticleShader : public Shader {
    public:
      explicit ParticleShader(utils::FileUtil* fileUtil);
      //void bindUniforms(Context &context) override;
      //void bindBuffer(Buffer *buffer) override;
  };
}

#endif //ALCUBE_PARTICLESHADER_H
