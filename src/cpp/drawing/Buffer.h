#define GL_GLEXT_PROTOTYPES
#ifndef ALCUBE_BUFFER_H
#define ALCUBE_BUFFER_H
#include <GL/glew.h>
#include <cstddef>

namespace alcube::drawing {
  class VBO {
    public:
      GLuint bufferId;
      void* data;
      GLsizeiptr allocationSize;
      GLsizeiptr size;
      GLenum target;
      GLint vertexSize;
      explicit VBO(GLsizeiptr allocationSize, GLenum target, GLint vertexSize);
  };

  class VBOs {
    public:
      VBO* vertices;
      VBO* indices;
      VBO* normals;
      VBO* colors;
      VBO* uvs;
      VBO* positions;
  };

  class Buffer {
    public:
      VBOs vbos;
      GLuint arrayId;
      explicit Buffer(
        GLsizeiptr verticesAllocationSize,
        GLsizeiptr indicesAllocationSize,
        GLsizeiptr normalsAllocationSize,
        GLsizeiptr colorsAllocationSize,
        GLsizeiptr uvsAllocationSize,
        GLsizeiptr positionsAllocationSize
      );

      ~Buffer();
  };
}

#endif //ALCUBE_BUFFER_H
