#ifndef ALCUBE_RESOURCESPROVIDER_H
#define ALCUBE_RESOURCESPROVIDER_H

#include "MemoryManager.h"
#include "ProgramFactory.h"
#include "KernelFactory.h"
#include "CommandQueue.h"

namespace alcube::utils::opencl {
  class ResourcesProvider {
    public:
      CommandQueue* queue;
      KernelFactory* kernelFactory;
      ProgramFactory* programFactory;
      explicit ResourcesProvider(FileUtil *fileUtil, Resources* resources);
      Resources* resources;
  };
}
#endif //ALCUBE_RESOURCESPROVIDER_H
