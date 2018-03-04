#include "Buffer.h"

namespace alcube::drawing {
  Buf::Buf(
    GLenum target,
    GLenum usage,
    size_t elementSize,
    unsigned int maxLength,
    unsigned int elementCountParVertex,
    void* data
  ) {
    this->data = data;
    this->elementSize = elementSize;
    this->length = maxLength;
    this->elementCountParVertex = elementCountParVertex;
    this->target = target;
    size_t allocationSize = maxLength * elementSize * elementCountParVertex;
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(target, this->bufferId);
    glBufferData(target, allocationSize, nullptr, usage);
  }

  void Buf::update() {
    size_t size = length * elementSize * elementCountParVertex;
    glBindBuffer(target, bufferId);
    glBufferSubData(target, 0, size, data);
    glBindBuffer(target, 0);
  }

  void Buf::disable() {
    glBindBuffer(target, 0);
  }

  VertexBuffer::VertexBuffer(
    unsigned int elementCountParVertex,
    unsigned int maxLength,
    void* data
  ): Buf(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(GLfloat), maxLength, elementCountParVertex, data) {
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
    Buf::disable();
    glDisableVertexAttribArray(location);
  }

  InstanceBuffer::InstanceBuffer(
    unsigned int elementCountParVertex,
    unsigned int maxLength,
    void* data
  ) : VertexBuffer(elementCountParVertex, maxLength, data) {}

  void InstanceBuffer::enable(GLuint location) {
    glVertexAttribDivisor(location, 1);
    VertexBuffer::enable(location);
  }

  IndexBuffer::IndexBuffer(
    unsigned int elementCountParVertex,
    unsigned int maxLength,
    unsigned int *data
  ) : Buf(
    GL_ELEMENT_ARRAY_BUFFER,
    GL_DYNAMIC_DRAW,
    sizeof(unsigned int),
    maxLength,
    elementCountParVertex,
    data
  ) {}

  void IndexBuffer::enable() {
    glBindBuffer(target, bufferId);
  }

  VBO::VBO(GLsizeiptr allocationSize, GLenum target, GLint vertexSize, GLenum usage) {
    this->target = target;
    this->allocationSize = allocationSize;
    this->size = allocationSize;
    this->vertexSize = vertexSize;
    data = nullptr;
    location = 0;
    isAttribute = false;
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(target, this->bufferId);
    glBufferData(target, allocationSize, nullptr, usage);
    enabled = false;
  }

  void VBO::update() {
    glBindBuffer(target, bufferId);
    glBufferSubData(target, 0, size, data);
    glBindBuffer(target, 0);
  }

  void VBO::enable() {
    enabled = true;
    isAttribute = false;
    glBindBuffer(target, bufferId);
  }

  void VBO::enable(GLuint location) {
    enabled = true;
    isAttribute = true;
    this->location = location;
    glEnableVertexAttribArray(location);
    glBindBuffer(target, bufferId);
    glVertexAttribPointer(
      (GLuint)location,
      vertexSize,
      GL_FLOAT,
      GL_FALSE,
      0,
      nullptr
    );
    glBindBuffer(target, 0);
  }

  void VBO::disable() {
    glBindBuffer(target, 0);
    if (isAttribute) {
      glDisableVertexAttribArray(location);
    }
    enabled = false;
  }

  Buffer::Buffer(
    GLsizeiptr verticesAllocationSize,
    GLsizeiptr indicesAllocationSize,
    GLsizeiptr normalsAllocationSize,
    GLsizeiptr colorsAllocationSize,
    GLsizeiptr uvsAllocationSize,
    GLsizeiptr positionsAllocationSize
  ) {
    vertices = nullptr;
    indices = nullptr;
    normals = nullptr;
    colors = nullptr;
    uvs = nullptr;
    positions = nullptr;

    if (verticesAllocationSize > 0) {
      vertices = new VBO(verticesAllocationSize, GL_ARRAY_BUFFER, 3, GL_STATIC_DRAW);
    }

    if (indicesAllocationSize > 0) {
      indices = new VBO(indicesAllocationSize, GL_ELEMENT_ARRAY_BUFFER, 3, GL_STATIC_DRAW);
    }

    if (normalsAllocationSize > 0) {
      normals = new VBO(normalsAllocationSize, GL_ARRAY_BUFFER, 3, GL_STATIC_DRAW);
    }

    if (colorsAllocationSize > 0) {
      colors = new VBO(colorsAllocationSize, GL_ARRAY_BUFFER, 3, GL_STATIC_DRAW);
    }

    if (uvsAllocationSize > 0) {
      uvs = new VBO(uvsAllocationSize, GL_ARRAY_BUFFER, 2, GL_STATIC_DRAW);
    }

    if (positionsAllocationSize > 0) {
      positions = new VBO(positionsAllocationSize, GL_ARRAY_BUFFER, 3, GL_DYNAMIC_DRAW);
    }

    all[0] = vertices;
    all[1] = indices;
    all[2] = normals;
    all[3] = colors;
    all[4] = uvs;
    all[5] = positions;
  }

  void Buffer::unbind() {
    for (auto vbo: all) {
      if (vbo != nullptr && vbo->enabled) {
        vbo->disable();
      }
    }
  }

  Buffer::~Buffer() {
    delete vertices;
    delete indices;
    delete normals;
    delete colors;
    delete uvs;
    delete positions;
  }
}
