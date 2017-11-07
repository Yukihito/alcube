#ifndef ALCUBE_MEMALLOCATOR_H
#define ALCUBE_MEMALLOCATOR_H

#include <OpenCL/opencl.h>
#include <string>
#include <iostream>

namespace alcube::utils::opencl {
  class MemAllocator {
    public:
      cl_mem mem;
      std::string name;
      size_t size;
      size_t count;
      cl_context context;
      cl_command_queue queue;
      void* ptr;
      virtual void allocate() = 0;
      virtual void read() = 0;
      void release();
      explicit MemAllocator(
        const std::string& name,
        size_t size,
        void* ptr,
        cl_context& context,
        cl_command_queue& queue
      );
  };

  class ReadableMemAllocator : public MemAllocator {
    public:
      void allocate() override;
      void read() override;
      explicit ReadableMemAllocator(
        const std::string& name,
        size_t size,
        void* ptr,
        cl_context& context,
        cl_command_queue& queue
      );
  };

  class WritableMemAllocator : public MemAllocator {
    public:
      void allocate() override;
      void read() override;
      explicit WritableMemAllocator(
        const std::string& name,
        size_t size,
        void* ptr,
        cl_context& context,
        cl_command_queue& queue
      );
  };
}
#endif //ALCUBE_MEMALLOCATOR_H
