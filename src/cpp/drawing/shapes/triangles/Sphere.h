#ifndef ALCUBE_SHAPES_TRIANGLES_SPHERE_H
#define ALCUBE_SHAPES_TRIANGLES_SPHERE_H

#include "../../Shape.h"
#include <cstddef>
#include <cmath>

namespace alcube::drawing::shapes::triangles {
  class Sphere : public Shape {
    public:
      void draw() override;
      explicit Sphere(
        size_t splitCount,
        unsigned int maxInstanceCount,
        GLfloat* positions
      );

    protected:
      size_t indicesLength;
  };
}

#endif //ALCUBE_SHAPES_TRIANGLES_SPHERE_H
