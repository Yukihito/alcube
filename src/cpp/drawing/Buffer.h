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
      GLuint location;
      bool isAttribute;
      bool enabled;
      explicit VBO(GLsizeiptr allocationSize, GLenum target, GLint vertexSize, GLenum usage);
      void update();
      void enable();
      void enable(GLuint location);
      void disable();
  };

  class Buf {
    public:
      explicit Buf(
        GLenum target,
        bool isDynamic,
        size_t elementSize,
        unsigned int maxLength,
        unsigned int elementCountParVertex,
        void* data
      );
      GLuint bufferId;
      void* data;
      unsigned int length;
      unsigned int elementCountParVertex;
      virtual void disable();
      bool isDynamic;
      void update();

    protected:
      size_t elementSize;
    protected:
      GLenum target;
  };


  class VertexBuffer : public Buf {
    public:
      explicit VertexBuffer(
        unsigned int elementCountParVertex,
        unsigned int maxLength,
        void* data,
        bool isDynamic
      );
      virtual void enable(GLuint location);
      void disable() override;

    protected:
      GLenum type;

    private:
      GLuint location;
  };

  class InstanceBuffer : public VertexBuffer {
    public:
      explicit InstanceBuffer(
        unsigned int elementCountParVertex,
        unsigned int maxLength,
        void* data,
        bool isDynamic
      );
      void enable(GLuint location) override;
  };

  class IndexBuffer : public Buf {
    public:
      explicit IndexBuffer(
        unsigned int elementCountParVertex,
        unsigned int maxLength,
        unsigned int* data,
        bool isDynamic
      );
      void enable();
  };

  class Buffer {
    public:
      VBO* vertices;
      VBO* indices;
      VBO* normals;
      VBO* colors;
      VBO* uvs;
      VBO* positions;
      explicit Buffer(
        GLsizeiptr verticesAllocationSize,
        GLsizeiptr indicesAllocationSize,
        GLsizeiptr normalsAllocationSize,
        GLsizeiptr colorsAllocationSize,
        GLsizeiptr uvsAllocationSize,
        GLsizeiptr positionsAllocationSize
      );

      ~Buffer();

      void unbind();

    private:
      VBO* all[6];
  };
}

#endif //ALCUBE_BUFFER_H
