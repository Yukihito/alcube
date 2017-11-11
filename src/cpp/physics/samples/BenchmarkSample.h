#ifndef ALCUBE_PHYSICS_BENCHMARKSAMPLE_H
#define ALCUBE_PHYSICS_BENCHMARKSAMPLE_H

#include <mutex>
#include <glm/vec3.hpp>
#include <iostream>
#include "../Simulator.h"
#include "../../utils/opencl/Resources.h"
#include "../../utils/FileUtil.h"
#include "../../utils/Profiler.h"

namespace alcube::physics::samples {
  class BenchmarkSample {
    public:
      void runApp(int argc, char *argv[]);
  };
}

#endif //ALCUBE_PHYSICS_BENCHMARKSAMPLE_H
