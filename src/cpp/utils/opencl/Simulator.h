#ifndef ALCUBE_OPENCL_SIMULATOR_H
#define ALCUBE_OPENCL_SIMULATOR_H

#include "../FileUtil.h"
#include "ProgramFactory.h"
#include "MemoryManager.h"
#include "KernelFactory.h"
#include "CommandQueue.h"
#include "ResourcesProvider.h"

namespace alcube::utils::opencl {
  class Simulator {
    public:
      explicit Simulator(ResourcesProvider* resourcesProvider);
      void tearDownMemories();
      void read(utils::opencl::Memory* memory, void* hostPtr);
      virtual void update(float deltaTime) = 0;
    protected:
      CommandQueue* queue;
      KernelFactory* kernelFactory;
      ProgramFactory* programFactory;
      MemoryManager* memoryManager;
  };
}

#endif //ALCUBE_OPENCL_SIMULATOR_H
