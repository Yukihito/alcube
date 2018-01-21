#include "Simulator.h"

namespace alcube::biology {
  Simulator::Simulator(
    utils::opencl::Resources *resources,
    utils::FileUtil *fileUtil
  ) : utils::opencl::Simulator(resources, fileUtil) {}
}