#ifndef ALCUBE_SHAPE_H
#define ALCUBE_SHAPE_H

#include <glm/mat4x4.hpp>
#include "Buffer.h"
//#include "Shader.h"

namespace alcube::drawing {
  enum VertexBufferType {
    VERTICES = 0,
    NORMALS,
    COLORS,
    UVS
  };

  enum InstanceBufferType {
    POSITIONS = 0
  };

  class Shape {
    public:
      explicit Shape();
      virtual void draw() = 0;
      Buffer* buffer = nullptr; // TODO: Delete
      IndexBuffer* indexBuffer;
      VertexBuffer* vertexBuffers[4];
      InstanceBuffer* instanceBuffers[1];

    protected:
      virtual Buffer* createBuffer() = 0; // TODO: Delete
      void initialize();
  };
}

#endif //ALCUBE_SHAPE_H
