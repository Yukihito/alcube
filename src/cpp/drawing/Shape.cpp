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

  void Shape::update() {
    if (indexBuffer != nullptr && indexBuffer->isDynamic) {
      indexBuffer->update();
    }

    for (auto &vertexBuffer : vertexBuffers) {
      if (vertexBuffer != nullptr && vertexBuffer->isDynamic) {
        vertexBuffer->update();
      }
    }
    for (auto &instanceBuffer : instanceBuffers) {
      if (instanceBuffer != nullptr && instanceBuffer->isDynamic) {
        instanceBuffer->length = instanceCount;
        instanceBuffer->update();
      }
    }
  }
}
