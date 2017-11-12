#ifndef ALCUBE_OPENCL_COMMANDQUEUE_H
#define ALCUBE_OPENCL_COMMANDQUEUE_H

#include "Resources.h"
#include "Memory.h"
#include <vector>

namespace alcube::utils::opencl {
  class KernelArg {
    public:
      size_t size;
      void* ptr;
  };

  namespace kernelargs {
    KernelArg memArg(Memory* mem);
    KernelArg uintArg(unsigned int& i);
  }

  class CommandQueue {
    public:
      explicit CommandQueue(Resources* resources);
      void push(cl_kernel op, std::vector<size_t> ranges, std::initializer_list<KernelArg> args);
      void pushZeroFill(Memory* mem);
      void read(Memory* memory, void* hostPtr);
    private:
      cl_command_queue queue;
      Resources* resources;
  };
}

#endif //ALCUBE_OPENCL_COMMANDQUEUE_H
