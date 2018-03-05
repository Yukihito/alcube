#ifndef ALCUBE_MULTISPHERE_H
#define ALCUBE_MULTISPHERE_H

#include "../../MultiShape.h"
#include <cstddef>
#include <cmath>

namespace alcube::drawing::shapes::triangles {
  class MultiSphere : public MultiShape {
    public:
      void draw() override;
      explicit MultiSphere(
        size_t splitCount,
        unsigned int maxInstanceCount,
        gpu::memories::Float3Memory* positionsMemory
      );

    protected:
      size_t indicesLength = 0;

    private:
      size_t splitCount;
  };
}

#endif //ALCUBE_MULTISPHERE_H
