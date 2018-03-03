#include "Buffer.h"

namespace alcube::drawing {
  VBO::VBO(GLsizeiptr allocationSize, GLenum target, GLint vertexSize, GLenum usage) {
    this->target = target;
    this->allocationSize = allocationSize;
    this->size = allocationSize;
    this->vertexSize = vertexSize;
    data = nullptr;
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(target, this->bufferId);
    glBufferData(target, allocationSize, nullptr, usage);
  }

  void VBO::update() {
    glBindBuffer(target, bufferId);
    glBufferSubData(target, 0, size, data);
  }

  void VBO::enable() {
    glBindBuffer(target, bufferId);
  }

  void VBO::enable(GLuint location) {
    glEnableVertexAttribArray(location);
    glBindBuffer(target, bufferId);
    glVertexAttribPointer(
      location,
      vertexSize,
      GL_FLOAT,
      GL_FALSE,
      0,
      nullptr
    );
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
