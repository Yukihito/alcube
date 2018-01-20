#include "Simulator.h"

namespace alcube::utils::opencl {
  Simulator::Simulator(Resources *resources, FileUtil *fileUtil) {
    kernelFactory = new utils::opencl::KernelFactory(resources, fileUtil);
    programFactory = new utils::opencl::ProgramFactory(resources, fileUtil);
    memoryManager = new utils::opencl::MemoryManager(resources);
    queue = new utils::opencl::CommandQueue(resources);
  }


  void Simulator::tearDownMemories() {
    memoryManager->release();
  }

  void Simulator::read(utils::opencl::Memory *memory, void *hostPtr) {
    queue->read(memory, hostPtr);
  }
}
