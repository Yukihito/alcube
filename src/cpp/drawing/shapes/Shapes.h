#ifndef ALCUBE_SHAPES_H
#define ALCUBE_SHAPES_H

#include "triangles/TrianglesProvider.h"
#include "points/PointsProvider.h"

namespace drawing::shapes {
  class Shapes {
    public:
      triangles::TrianglesProvider triangles;
      points::PointsProvider points;
      explicit Shapes();
  };
}

#endif //ALCUBE_SHAPES_H
