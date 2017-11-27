#ifndef ALCUBE_BUFFER_H
#define ALCUBE_BUFFER_H
#include <OpenGL/gl3.h>
#include <cstddef>

namespace alcube::drawing {
  class VBO {
    public:
      GLuint bufferId;
      void* data;
      GLsizeiptr allocationSize;
      GLsizeiptr size;
      GLenum target;
      explicit VBO(GLsizeiptr allocationSize, GLenum target);
  };

  class VBOs {
    public:
      VBO* vertices;
      VBO* indices;
      VBO* normals;
      VBO* colors;
  };

  class Buffer {
    public:
      VBOs vbos;
      GLuint arrayId;

      /*
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
*/
      explicit Buffer(
        GLsizeiptr verticesAllocationSize,
        GLsizeiptr indicesAllocationSize,
        GLsizeiptr normalsAllocationSize,
        GLsizeiptr colorsAllocationSize
      );

      ~Buffer();
  };
}

#endif //ALCUBE_BUFFER_H
