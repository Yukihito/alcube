#ifndef ALCUBE_TRIANGLES_PROVIDER_H
#define ALCUBE_TRIANGLES_PROVIDER_H

#include "Sphere.h"

namespace drawing::shapes::triangles {
  class TrianglesProvider {
    public:
      Sphere sphere;
      explicit TrianglesProvider();
  };
}

#endif //ALCUBE_TRIANGLES_PROVIDER_H
