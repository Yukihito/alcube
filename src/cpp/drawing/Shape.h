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
    ROTATIONS_0,
    ROTATIONS_1,
    ROTATIONS_2,
    ROTATIONS_3,
    COLORS
  };

  class Shape {
    public:
      explicit Shape();
      virtual void draw() = 0;
      virtual void setUp() = 0;
      unsigned int instanceCount = 0;
      IndexBuffer* indexBuffer;
      VertexBuffer* vertexBuffers[3];
      InstanceBuffer* instanceBuffers[6];
      bool isReady = false;
      void update();
  };
}

#endif //ALCUBE_SHAPE_H
