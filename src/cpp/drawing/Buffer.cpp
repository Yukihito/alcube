#include "Buffer.h"

namespace alcube::drawing {
  VBO::VBO(GLsizeiptr allocationSize, GLenum target, GLint vertexSize) {
    this->target = target;
    this->allocationSize = allocationSize;
    this->size = allocationSize;
    this->vertexSize = vertexSize;
    data = nullptr;
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(target, this->bufferId);
    glBufferData(target, allocationSize, nullptr, GL_DYNAMIC_DRAW);
  }

  Buffer::Buffer(
    GLsizeiptr verticesAllocationSize,
    GLsizeiptr indicesAllocationSize,
    GLsizeiptr normalsAllocationSize,
    GLsizeiptr colorsAllocationSize,
    GLsizeiptr uvsAllocationSize
  ) {
    vbos.vertices = nullptr;
    vbos.indices = nullptr;
    vbos.normals = nullptr;
    vbos.colors = nullptr;
    vbos.uvs = nullptr;

    glGenVertexArrays(1, &this->arrayId);
    glBindVertexArray(this->arrayId);

    if (verticesAllocationSize > 0) {
      vbos.vertices = new VBO(verticesAllocationSize, GL_ARRAY_BUFFER, 3);
    }

    if (indicesAllocationSize > 0) {
      vbos.indices = new VBO(indicesAllocationSize, GL_ELEMENT_ARRAY_BUFFER, 3);
    }

    if (normalsAllocationSize > 0) {
      vbos.normals = new VBO(normalsAllocationSize, GL_ARRAY_BUFFER, 3);
    }

    if (colorsAllocationSize > 0) {
      vbos.colors = new VBO(colorsAllocationSize, GL_ARRAY_BUFFER, 3);
    }

    if (uvsAllocationSize > 0) {
      vbos.uvs = new VBO(uvsAllocationSize, GL_ARRAY_BUFFER, 2);
    }

    glBindVertexArray(0);
  }

  Buffer::~Buffer() {
    delete vbos.vertices;
    delete vbos.indices;
    delete vbos.normals;
    delete vbos.colors;
    delete vbos.uvs;
  }
}
