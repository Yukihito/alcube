#include "Simulator.h"

namespace alcube::utils::opencl {
  Simulator::Simulator(ResourcesProvider *resourcesProvider) {
    kernelFactory = resourcesProvider->kernelFactory;
    programFactory = resourcesProvider->programFactory;
    memoryManager = resourcesProvider->memoryManager;
    queue = resourcesProvider->queue;
  }


  void Simulator::tearDownMemories() {
    memoryManager->release();
  }

  void Simulator::read(utils::opencl::Memory *memory, void *hostPtr) {
    queue->read(memory, hostPtr);
  }
}
