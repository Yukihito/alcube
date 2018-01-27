#ifndef ALCUBE_BIOLOGY_OPENCL_DTOS_H
#define ALCUBE_BIOLOGY_OPENCL_DTOS_H

#include <CL/cl.h>

namespace alcube::biology::opencl::dtos {
  class BiologyStatus {
    public:
      float mass;
      float elasticity;
      float dynamicFrictionCoefficient;
      float staticFrictionCoefficient;
      unsigned int springIndices[16];
      unsigned char springNodeIndices[16];
      unsigned int springCount;
      int alterEgoIndex;
      float radiusForAlterEgo;
  };
}

#endif //ALCUBE_BIOLOGY_OPENCL_DTOS_H
