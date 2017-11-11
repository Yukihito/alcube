#ifndef ALCUBE_OPENCL_MEMORY_H
#define ALCUBE_OPENCL_MEMORY_H

#include <OpenCL/opencl.h>
#include <string>
#include <iostream>

namespace alcube::utils::opencl {
  class Memory {
    public:
      cl_mem mem;
      std::string name;
      size_t size;
      size_t count;
      cl_context context;
      void allocate();
      void release();
      explicit Memory(
        const std::string& name,
        size_t size,
        cl_context& context,
        void* hostPtr,
        bool isReadable,
        bool isWritable
      );
    private:
      void* hostPtr;
      cl_mem_flags memFlags;
  };
}
#endif //ALCUBE_OPENCL_MEMORY_H
