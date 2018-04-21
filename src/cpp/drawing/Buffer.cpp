#include "Buffer.h"

namespace alcube::drawing {
  Buffer::Buffer(
    GLenum target,
    bool isDynamic,
    size_t elementSize,
    unsigned int maxLength,
    unsigned int elementCountParVertex,
    void* data
  ) {
    GLenum usage = isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    this->data = data;
    this->elementSize = elementSize;
    this->length = maxLength;
    this->elementCountParVertex = elementCountParVertex;
    this->target = target;
    this->isDynamic = isDynamic;
    size_t allocationSize = maxLength * elementSize * elementCountParVertex;
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(target, this->bufferId);
    glBufferData(target, allocationSize, nullptr, usage);
  }

  void Buffer::update() {
    size_t size = length * elementSize * elementCountParVertex;
    glBindBuffer(target, bufferId);
    glBufferSubData(target, 0, size, data);
    glBindBuffer(target, 0);
  }

  void Buffer::setLength(unsigned int length) {
    this->length = length;
  }

  bool Buffer::shouldUpdateDynamically() {
    return data != nullptr && isDynamic;
  }

  void Buffer::disable() {
    glBindBuffer(target, 0);
  }

  VertexBuffer::VertexBuffer(
    unsigned int elementCountParVertex,
    unsigned int maxLength,
    void* data,
    bool isDynamic
  ): Buffer(GL_ARRAY_BUFFER, isDynamic, sizeof(GLfloat), maxLength, elementCountParVertex, data) {
    this->type = GL_FLOAT;
    location = 0;
  }

  void VertexBuffer::enable(GLuint location) {
    this->location = location;
    glEnableVertexAttribArray(location);
    glBindBuffer(target, bufferId);
    glVertexAttribPointer(
      location,
      elementCountParVertex,
      type,
      GL_FALSE,
      0,
      nullptr
    );
  }

  void VertexBuffer::disable() {
    Buffer::disable();
    glDisableVertexAttribArray(location);
  }

  InstanceBuffer::InstanceBuffer(
    unsigned int elementCountParVertex,
    unsigned int maxLength,
    void* data,
    bool isDynamic
  ) : VertexBuffer(elementCountParVertex, maxLength, data, isDynamic) {}

  void InstanceBuffer::enable(GLuint location) {
    glVertexAttribDivisor(location, 1);
    VertexBuffer::enable(location);
  }

  IndexBuffer::IndexBuffer(
    unsigned int elementCountParVertex,
    unsigned int maxLength,
    unsigned int *data,
    bool isDynamic
  ) : Buffer(
    GL_ELEMENT_ARRAY_BUFFER,
    isDynamic,
    sizeof(unsigned int),
    maxLength,
    elementCountParVertex,
    data
  ) {}

  void IndexBuffer::enable() {
    glBindBuffer(target, bufferId);
  }
}
