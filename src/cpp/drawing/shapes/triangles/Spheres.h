#ifndef ALCUBE_TRIANGLES_SPHERES_H
#define ALCUBE_TRIANGLES_SPHERES_H

#include "../../GroupShape.h"
#include <cstddef>
#include <cmath>
#include <CL/cl.h>


namespace alcube::drawing::shapes::triangles {
  class Spheres : public GroupShape {
    public:
      size_t indicesLength = 0;
      void draw() override;
      explicit Spheres(
        size_t splitCount,
        unsigned int maxModelCount,
        gpu::memories::Float3Memory* hostModelVerticesMemory,
        gpu::memories::Float3Memory* modelVerticesMemory,
        gpu::memories::Float3Memory* verticesMemory
      );

    protected:

      Buffer* createBuffer() override;

    private:
      size_t splitCount;
  };
}

#endif //ALCUBE_TRIANGLES_SPHERES_H
