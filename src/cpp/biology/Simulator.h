#ifndef ALCUBE_BIOLOGY_SIMULATOR_H
#define ALCUBE_BIOLOGY_SIMULATOR_H


#include "../utils/opencl/ResourcesProvider.h"

namespace alcube::biology {
  class Simulator {
    public:
      explicit Simulator(utils::opencl::ResourcesProvider *resourcesProvider);
  };
}

#endif //ALCUBE_BIOLOGY_SIMULATOR_H
