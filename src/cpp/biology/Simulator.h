#ifndef ALCUBE_BIOLOGY_SIMULATOR_H
#define ALCUBE_BIOLOGY_SIMULATOR_H

#include "../physics/Simulator.h"

namespace alcube::biology {
  class Simulator : utils::opencl::Simulator {
    public:
      explicit Simulator(utils::opencl::ResourcesProvider *resourcesProvider);
  };
}

#endif //ALCUBE_BIOLOGY_SIMULATOR_H
