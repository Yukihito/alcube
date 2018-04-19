#ifndef ALCUBE_DRAWING_IMAGE_H
#define ALCUBE_DRAWING_IMAGE_H
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace alcube::drawing {
  class Texture {
    public:
      explicit Texture(unsigned int width, unsigned int height);
      void bind();
    protected:
      GLuint id;
      unsigned char* data;
      unsigned int width;
      unsigned int height;
  };
}

#endif //ALCUBE_DRAWING_IMAGE_H
