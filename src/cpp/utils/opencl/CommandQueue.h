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
    KernelArg ushortArg(unsigned short& i);
    KernelArg floatArg(float& f);
  }

  class CommandQueue {
    public:
      cl_command_queue queue;
      explicit CommandQueue(Resources* resources);
      void push(cl_kernel op, std::vector<size_t> ranges, std::initializer_list<KernelArg> args);
      void pushZeroFill(Memory* mem);
      void read(Memory* memory, void* hostPtr);
      void readRange(Memory* memory, void* hostPtr, size_t min, size_t length);
      void write(Memory* memory);
      void write(Memory* memory, size_t offset);
      void readAt(Memory* memory, void* hostPtr, unsigned int index);
    private:
      Resources* resources;
  };
}

#endif //ALCUBE_OPENCL_COMMANDQUEUE_H
