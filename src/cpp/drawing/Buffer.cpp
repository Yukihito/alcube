#include "Buffer.h"

namespace alcube::drawing {
  Buffer::Buffer(size_t verticesSize, size_t indicesSize, size_t normalsSize) {
    this->vertexBufferId = 0;
    this->indexBufferId = 0;
    this->normalBufferId = 0;
    this->arrayId = 0;
    this->vertices = nullptr;
    this->indices = nullptr;
    this->normals = nullptr;

    this->verticesSize = verticesSize;
    this->indicesSize = indicesSize;
    this->normalsSize = normalsSize;

    glGenVertexArrays(1, &this->arrayId);
    glBindVertexArray(this->arrayId);

    glGenBuffers(1, &this->vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)verticesSize, nullptr, GL_DYNAMIC_DRAW);

    if (indicesSize > 0) {
      glGenBuffers(1, &this->indexBufferId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indicesSize, nullptr, GL_DYNAMIC_DRAW);
    }

    if (normalsSize > 0) {
      glGenBuffers(1, &this->normalBufferId);
      glBindBuffer(GL_ARRAY_BUFFER, this->normalBufferId);
      glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)normalsSize, nullptr, GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(0);
  }

  Buffer::~Buffer() {
    delete this->vertices;
    delete this->indices;
  }
}
