#ifndef ALCUBE_OPENCL_MEMORYMANAGER_H
#define ALCUBE_OPENCL_MEMORYMANAGER_H

#include "Memory.h"
#include "Resources.h"
#include <vector>

namespace alcube::utils::opencl {
  class MemoryManager {
    public:
      Memory* define(
        const std::string& name,
        size_t size,
        void* hostPtr,
        bool isReadable,
        bool isWritable
      );
      void allocate();
      void release();
      explicit MemoryManager(Resources* resources);
    private:
      Resources* resources;
      std::vector<Memory*> memories = {};
  };
}

#endif //ALCUBE_OPENCL_MEMORYMANAGER_H
