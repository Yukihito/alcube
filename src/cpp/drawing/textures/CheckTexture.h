#ifndef ALCUBE_CHECKTEXTURE_H
#define ALCUBE_CHECKTEXTURE_H


#include "../Texture.h"

namespace alcube::drawing::textures {
  class CheckTexture : public Texture {
      explicit CheckTexture(unsigned int width, unsigned int height);
  };
}


#endif //ALCUBE_CHECKTEXTURE_H
