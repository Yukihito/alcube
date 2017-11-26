#ifndef ALCUBE_BUFFER_H
#define ALCUBE_BUFFER_H
#include <OpenGL/gl3.h>
#include <cstddef>

namespace alcube::drawing {
  class Buffer {
    public:
      GLuint arrayId;

      GLuint vertexBufferId;
      GLuint indexBufferId;
      GLuint normalBufferId;
      GLuint colorBufferId;

      GLfloat *vertices;
      GLuint *indices;
      GLfloat *normals;
      GLfloat *colors;

      size_t verticesSize;
      size_t indicesSize;
      size_t normalsSize;
      size_t colorsSize;

      size_t verticesAllocationSize;
      size_t indicesAllocationSize;
      size_t normalsAllocationSize;
      size_t colorsAllocationSize;

      explicit Buffer(size_t verticesAllocationSize, size_t indicesAllocationSize, size_t normalsAllocationSize, size_t colorsAllocationSize);

      ~Buffer();
  };
}

#endif //ALCUBE_BUFFER_H
