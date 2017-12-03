#include "Sphere.h"

namespace alcube::drawing::shapes::points {
  void Sphere::draw() {
    glDrawArrays(GL_POINTS, 0, (int)splitCount * (int)splitCount);
  };

  Buffer* Sphere::createBuffer() {
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

    auto buffer = new Buffer(verticesLength * sizeof(GLfloat), 0, 0, 0, 0);
    buffer->vbos.vertices->data = vertices;
    return buffer;
  }

  Sphere::Sphere(size_t splitCount) {
    this->splitCount = splitCount;
    initialize();
  }
}