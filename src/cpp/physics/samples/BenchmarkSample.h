#ifndef ALCUBE_PHYSICS_BENCHMARKSAMPLE_H
#define ALCUBE_PHYSICS_BENCHMARKSAMPLE_H

#include <mutex>
#include <glm/vec3.hpp>
#include <iostream>
#include "../Simulator.h"
#include "../../utils/opencl/Resources.h"
#include "../../utils/FileUtil.h"
#include "../../utils/Profiler.h"
#include "../../utils/app/Application.h"

namespace alcube::physics::samples {
  class BenchmarkSample : public utils::app::Application {
    public:
      void runApp(int argc, char *argv[]) override;
  };
}

#endif //ALCUBE_PHYSICS_BENCHMARKSAMPLE_H
