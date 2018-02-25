#ifndef ALCUBE_GROUPSHAPE_H
#define ALCUBE_GROUPSHAPE_H

#include "Shape.h"
#include "../gpu/GPU.h"

namespace alcube::drawing {
  class GroupShape : public Shape {
    public:
      size_t modelVertexCount = 0;
      unsigned int maxModelCount = 0;
      unsigned int modelCount = 0;
      float* modelVertices = nullptr;
      gpu::memories::Float3Memory* hostModelVerticesMemory = nullptr;
      gpu::memories::Float3Memory* modelVerticesMemory = nullptr;
      gpu::memories::Float3Memory* verticesMemory = nullptr;
  };
}

#endif //ALCUBE_GROUPSHAPE_H
