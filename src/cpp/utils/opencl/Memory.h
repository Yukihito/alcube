#ifndef ALCUBE_OPENCL_MEMORY_H
#define ALCUBE_OPENCL_MEMORY_H

#include <CL/opencl.h>
#include <string>
#include <iostream>

namespace alcube::utils::opencl {
  class Memory {
    public:
      cl_mem mem;
      std::string name;
      size_t size;
      size_t count;
      size_t allocationCount;
      cl_context context;
      void* hostPtr;
      void allocate();
      void release();
      explicit Memory(
        const std::string& name,
        size_t size,
        cl_context& context,
        void* hostPtr,
        cl_mem_flags memFlags
      );
    private:
      cl_mem_flags memFlags;
  };
}
#endif //ALCUBE_OPENCL_MEMORY_H
