#include "Simulator.h"

namespace alcube::biology {
  Simulator::Simulator(
    utils::opencl::ResourcesProvider *resourcesProvider
  ) : utils::opencl::Simulator(resourcesProvider) {}
}