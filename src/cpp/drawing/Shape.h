#ifndef ALCUBE_SHAPE_H
#define ALCUBE_SHAPE_H

#include <glm/mat4x4.hpp>
#include "Buffer.h"

namespace alcube::drawing {
  enum VertexBufferType {
    VERTICES = 0,
    NORMALS,
    UVS
  };

  enum InstanceBufferType {
    POSITIONS = 0,
    ROTATIONS = 0,
    COLORS
  };

  class Shape {
    public:
      explicit Shape();
      virtual void draw() = 0;
      unsigned int instanceCount = 0;
      IndexBuffer* indexBuffer;
      VertexBuffer* vertexBuffers[3];
      InstanceBuffer* instanceBuffers[3];
      void update();
  };
}

#endif //ALCUBE_SHAPE_H
