#ifndef ALCUBE_MULTISHAPE_H
#define ALCUBE_MULTISHAPE_H

#include "Shape.h"
#include "../gpu/GPU.h"

namespace alcube::drawing {
  class MultiShape : public Shape {
    public:
      unsigned int instanceCount = 0;
      gpu::memories::Float3Memory* positionsMemory = nullptr;
  };
}

#endif //ALCUBE_MULTISHAPE_H
