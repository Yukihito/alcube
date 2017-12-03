#ifndef ALCUBE_TILESHADER_H
#define ALCUBE_TILESHADER_H

#include "../Shader.h"

namespace alcube::drawing::shaders {
  class TileShader : public Shader {
    public:
      explicit TileShader(utils::FileUtil *fileUtil);

      void shade(Context &context) override;

      void setupBuffer(Buffer *buffer) override;
  };
}



#endif //ALCUBE_TILESHADER_H
