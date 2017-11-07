#ifndef ALCUBE_POINTSPROVIDER_H
#define ALCUBE_POINTSPROVIDER_H

#include "Sphere.h"

namespace alcube::drawing::shapes::points {
  class PointsProvider {
    public:
      Sphere sphere;
      explicit PointsProvider();
  };
}

#endif //ALCUBE_POINTSPROVIDER_H
