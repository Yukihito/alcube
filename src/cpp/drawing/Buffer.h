#ifndef ALCUBE_BUFFER_H
#define ALCUBE_BUFFER_H
#include <OpenGL/gl3.h>
#include <cstddef>

namespace drawing {
  class Buffer {
    public:
      GLuint arrayId;
      GLuint vertexBufferId;
      GLuint indexBufferId;
      GLuint normalBufferId;
      GLfloat *vertices;
      GLuint *indices;
      GLfloat *normals;
      size_t verticesSize;
      size_t indicesSize;
      size_t normalsSize;

      explicit Buffer(size_t verticesSize, size_t indicesSize, size_t normalsSize);

      ~Buffer();
  };
}

#endif //ALCUBE_BUFFER_H
