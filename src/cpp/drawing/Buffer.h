#define GL_GLEXT_PROTOTYPES
#ifndef ALCUBE_BUFFER_H
#define ALCUBE_BUFFER_H
#include <GL/glew.h>
#include <cstddef>

namespace alcube::drawing {
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
}

#endif //ALCUBE_BUFFER_H
