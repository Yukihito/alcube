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
    if (indexBuffer != nullptr && indexBuffer->shouldUpdateDynamically()) {
      indexBuffer->update();
    }

    for (auto &vertexBuffer : vertexBuffers) {
      if (vertexBuffer != nullptr && vertexBuffer->shouldUpdateDynamically()) {
        vertexBuffer->update();
      }
    }
    for (auto &instanceBuffer : instanceBuffers) {
      if (instanceBuffer != nullptr && instanceBuffer->shouldUpdateDynamically()) {
        instanceBuffer->setLength(instanceCount);
        instanceBuffer->update();
      }
    }
  }
}
