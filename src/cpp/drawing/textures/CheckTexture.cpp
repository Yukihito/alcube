#include "CheckTexture.h"

namespace alcube::drawing::textures {
  CheckTexture::CheckTexture(unsigned int width, unsigned int height) : Texture(width, height) {
    unsigned int checkCount = 16;
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
        unsigned int i = (x + y * height) * 3;
        bool isBlack = x % (checkCount * 2) > checkCount ^ y % (checkCount * 2) > checkCount;
        unsigned char color = isBlack ? (unsigned char)0 : (unsigned char)255;
        for (unsigned int j = 0; j < 3; j++) {
          data[i + j] = color;
        }
      }
    }
  }
}
