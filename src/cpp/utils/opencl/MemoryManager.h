#ifndef ALCUBE_OPENCL_MEMORYMANAGER_H
#define ALCUBE_OPENCL_MEMORYMANAGER_H

#include "Memory.h"
#include <vector>
#include <CL/cl.h>

namespace alcube::utils::opencl {
  class MemoryManager {
    public:
      Memory* define(
        const std::string& name,
        size_t size,
        void* hostPtr,
        cl_mem_flags memFlags,
        size_t allocationCount
      );
      cl_context context;
      void allocate();
      void release();
    private:
      std::vector<Memory*> memories = {};
  };
}

#endif //ALCUBE_OPENCL_MEMORYMANAGER_H
