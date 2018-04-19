#include "Texture.h"

namespace alcube::drawing {
  Texture::Texture(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    this->data = new unsigned char[width * height * 3];
    for (int i = 0; i < width * height * 3; i++) {
      data[i] = 0;
    }
    glGenTextures(1, &id);
  }

  void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
}
