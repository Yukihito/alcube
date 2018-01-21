#ifndef ALCUBE_BIOLOGY_SIMULATOR_H
#define ALCUBE_BIOLOGY_SIMULATOR_H

#include "../physics/Simulator.h"

namespace alcube::biology {
  class Simulator : utils::opencl::Simulator {
    public:
      Simulator(utils::opencl::Resources *resources, utils::FileUtil *fileUtil);

  };
}

#endif //ALCUBE_BIOLOGY_SIMULATOR_H
