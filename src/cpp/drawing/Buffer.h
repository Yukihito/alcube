#ifndef ALCUBE_BUFFER_H
#define ALCUBE_BUFFER_H
#include <GL/glew.h>

namespace alcube::drawing {
  class Buffer {
    public:
      explicit Buffer(
        GLenum target,
        bool isDynamic,
        size_t elementSize,
        unsigned int maxLength,
        unsigned int elementCountParVertex,
        void* data
      );
      virtual void disable();
      void update();
      void setLength(unsigned int length);
      bool shouldUpdateDynamically();

    protected:
      GLuint bufferId;
      GLenum target;
      unsigned int elementCountParVertex;

    private:
      bool isDynamic;
      void* data;
      size_t elementSize;
      unsigned int length;
  };


  class VertexBuffer : public Buffer {
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

  class IndexBuffer : public Buffer {
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
