#ifndef ALCUBE_TRIANGLES_SPHERE_H
#define ALCUBE_TRIANGLES_SPHERE_H

#include "../../Shape.h"
#include <cstddef>
#include <cmath>

namespace alcube::drawing::shapes::triangles {
  class Sphere : public Shape {
    public:
      void draw() override;
      explicit Sphere(size_t splitCount);

    protected:
      size_t indicesLength = 0;
      Buffer* createBuffer() override;

    private:
      size_t splitCount;
  };
}

#endif //ALCUBE_TRIANGLES_SPHERE_H
