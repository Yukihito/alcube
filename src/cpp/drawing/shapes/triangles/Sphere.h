#ifndef ALCUBE_SHAPES_TRIANGLES_SPHERE_H
#define ALCUBE_SHAPES_TRIANGLES_SPHERE_H

#include "../../Shape.h"
#include <cstddef>
#include <cmath>

namespace alcube::drawing::shapes::triangles {
  class Sphere : public Shape {
    public:
      void draw() override;
      void setUp() override;
      explicit Sphere(
        size_t splitCount,
        unsigned int maxInstanceCount,
        GLfloat* positions,
        GLfloat* rotations0,
        GLfloat* rotations1,
        GLfloat* rotations2,
        GLfloat* rotations3,
        GLfloat* colors
      );

    protected:
      size_t indicesLength;
      GLfloat* positionsBuf;

    private:
      size_t splitCount;
      unsigned int maxInstanceCount;
      GLfloat* positions;
      GLfloat* rotations0;
      GLfloat* rotations1;
      GLfloat* rotations2;
      GLfloat* rotations3;
      GLfloat* colors;
  };
}

#endif //ALCUBE_SHAPES_TRIANGLES_SPHERE_H
