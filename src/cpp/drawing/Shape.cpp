#include "Shape.h"

namespace alcube::drawing {
  Shape::Shape() {
    indexBuffer = nullptr;
    for (auto &vertexBuffer : vertexBuffers) {
      vertexBuffer = nullptr;
    }
    for (auto &instanceBuffer : instanceBuffers) {
      instanceBuffer = nullptr;
    }
  }

  void Shape::initialize() {
    this->buffer = createBuffer();
  }
}
