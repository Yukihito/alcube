#include "Sphere.h"

namespace alcube::drawing::shapes::triangles {
  void Sphere::draw() {
    glDrawElements(GL_TRIANGLES, (int)this->indicesLength, GL_UNSIGNED_INT, nullptr);
  };

  Buffer* Sphere::createBuffer() {
    auto iSplitCount = (int)splitCount;
    size_t verticesLength = splitCount * splitCount * 3;
    auto vertices = new GLfloat[verticesLength]();
    auto normals = new GLfloat[verticesLength]();
    float pi = 3.1415f;
    for (int i = 0; i < splitCount * 3; i+=3) {
      float y = (float)i / (float)(splitCount * 3);
      float r = std::sin(y * pi);
      for (int j = 0; j < splitCount * 3; j+=3) {
        float x = (float)j / (float)(splitCount * 3);
        size_t k = i * splitCount + j;
        vertices[k + 0] = std::cos(x * pi * 2) * r;
        vertices[k + 1] = std::cos(y * pi);
        vertices[k + 2] = std::sin(x * pi * 2) * r;
        normals[k + 0] = std::cos(x * pi * 2) * r;
        normals[k + 1] = std::cos(y * pi);
        normals[k + 2] = std::sin(x * pi * 2) * r;
      }
    }
    this->indicesLength = splitCount * splitCount * 6;
    auto indices = new GLuint[indicesLength]();
    int indicesIndex = 0;
    for (int i = 0; i < splitCount; i++) {
      for (int j = 0; j < splitCount; j++) {
        indices[indicesIndex + 0] = (uint)(i * iSplitCount + j);
        indices[indicesIndex + 2] = (uint)((i + 1) * iSplitCount + j);
        indices[indicesIndex + 1] = (uint)(i * iSplitCount + j + 1);
        indices[indicesIndex + 3] = (uint)(i * iSplitCount + j + 1);
        indices[indicesIndex + 5] = (uint)((i + 1) * iSplitCount + j);
        indices[indicesIndex + 4] = (uint)((i + 1) * iSplitCount + j + 1);
        indicesIndex += 6;
      }
    }
    auto buffer = new Buffer(
      verticesLength * sizeof(GLfloat),
      indicesLength * sizeof(GLuint),
      verticesLength * sizeof(GLfloat),
      0
    );
    buffer->vbos.vertices->data = vertices;
    buffer->vbos.indices->data = indices;
    buffer->vbos.normals->data = normals;
    return buffer;
  }

  Sphere::Sphere(size_t splitCount) {
    this->splitCount = splitCount;
    initialize();
  }
}
