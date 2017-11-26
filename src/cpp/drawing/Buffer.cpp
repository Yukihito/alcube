#include "Buffer.h"

namespace alcube::drawing {
  Buffer::Buffer(size_t verticesAllocationSize, size_t indicesAllocationSize, size_t normalsAllocationSize, size_t colorsAllocationSize) {
    this->arrayId = 0;
    this->vertexBufferId = 0;
    this->indexBufferId = 0;
    this->normalBufferId = 0;
    this->colorBufferId = 0;

    this->vertices = nullptr;
    this->indices = nullptr;
    this->normals = nullptr;
    this->colors = nullptr;

    this->verticesSize = verticesAllocationSize;
    this->indicesSize = indicesAllocationSize;
    this->normalsSize = normalsAllocationSize;
    this->colorsSize = colorsAllocationSize;

    this->verticesAllocationSize = verticesAllocationSize;
    this->indicesAllocationSize = indicesAllocationSize;
    this->normalsAllocationSize = normalsAllocationSize;
    this->colorsAllocationSize = colorsAllocationSize;

    glGenVertexArrays(1, &this->arrayId);
    glBindVertexArray(this->arrayId);

    glGenBuffers(1, &this->vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)verticesAllocationSize, nullptr, GL_DYNAMIC_DRAW);

    if (indicesAllocationSize > 0) {
      glGenBuffers(1, &this->indexBufferId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indicesAllocationSize, nullptr, GL_DYNAMIC_DRAW);
    }

    if (normalsAllocationSize > 0) {
      glGenBuffers(1, &this->normalBufferId);
      glBindBuffer(GL_ARRAY_BUFFER, this->normalBufferId);
      glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)normalsAllocationSize, nullptr, GL_DYNAMIC_DRAW);
    }

    if (colorsAllocationSize > 0) {
      glGenBuffers(1, &this->colorBufferId);
      glBindBuffer(GL_ARRAY_BUFFER, this->colorBufferId);
      glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)colorsAllocationSize, nullptr, GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(0);
  }

  Buffer::~Buffer() {
    delete this->vertices;
    delete this->indices;
    delete this->normals;
    delete this->colors;
  }
}
