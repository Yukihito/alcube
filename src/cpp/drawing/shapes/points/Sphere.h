#ifndef ALCUBE_POINTS_SPHERE_H
#define ALCUBE_POINTS_SPHERE_H

#include "../../Shape.h"
#include <cstddef>

namespace alcube::drawing::shapes::points {
  class Sphere : public Shape {
    public:
      void draw() override;
      explicit Sphere(size_t splitCount);

    protected:
      Buffer* createBuffer() override;

    private:
      size_t splitCount;
  };
}


#endif //ALCUBE_POINTS_SPHERE_H
