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
        GLfloat* positions
      );

    protected:
      size_t indicesLength;
  };
}

#endif //ALCUBE_MULTISPHERE_H
