#include "Buffer.h"

namespace alcube::drawing {
  VBO::VBO(GLsizeiptr allocationSize, GLenum target) {
    this->target = target;
    this->allocationSize = allocationSize;
    this->size = allocationSize;
    data = nullptr;
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(target, this->bufferId);
    glBufferData(target, allocationSize, nullptr, GL_DYNAMIC_DRAW);
  }

  Buffer::Buffer(
    GLsizeiptr verticesAllocationSize,
    GLsizeiptr indicesAllocationSize,
    GLsizeiptr normalsAllocationSize,
    GLsizeiptr colorsAllocationSize
  ) {
    vbos.vertices = nullptr;
    vbos.indices = nullptr;
    vbos.normals = nullptr;
    vbos.colors = nullptr;

    glGenVertexArrays(1, &this->arrayId);
    glBindVertexArray(this->arrayId);

    if (verticesAllocationSize > 0) {
      vbos.vertices = new VBO(verticesAllocationSize, GL_ARRAY_BUFFER);
    }

    if (indicesAllocationSize > 0) {
      vbos.indices = new VBO(indicesAllocationSize, GL_ELEMENT_ARRAY_BUFFER);
    }

    if (normalsAllocationSize > 0) {
      vbos.normals = new VBO(normalsAllocationSize, GL_ARRAY_BUFFER);
    }

    if (colorsAllocationSize > 0) {
      vbos.colors = new VBO(colorsAllocationSize, GL_ARRAY_BUFFER);
    }

    glBindVertexArray(0);
  }

  Buffer::~Buffer() {
    delete vbos.vertices;
    delete vbos.indices;
    delete vbos.normals;
    delete vbos.colors;
  }
}
