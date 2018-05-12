#include "CommandQueue.h"

namespace alcube::utils::opencl {
  KernelArg kernelargs::memArg(Memory *mem) {
    KernelArg arg = KernelArg();
    arg.size = sizeof(cl_mem);
    arg.ptr = &mem->mem;
    return arg;
  }

  KernelArg kernelargs::uintArg(unsigned int& i) {
    KernelArg arg = KernelArg();
    arg.size = sizeof(cl_uint);
    arg.ptr = &i;
    return arg;
  }

  KernelArg kernelargs::ushortArg(unsigned short& i) {
    KernelArg arg = KernelArg();
    arg.size = sizeof(cl_ushort);
    arg.ptr = &i;
    return arg;
  }

  KernelArg kernelargs::floatArg(float& f) {
    KernelArg arg = KernelArg();
    arg.size = sizeof(cl_float);
    arg.ptr = &f;
    return arg;
  }

  CommandQueue::CommandQueue(Resources *resources) {
    this->resources = resources;
    this->queue = clCreateCommandQueueWithProperties(
      this->resources->context,
      this->resources->deviceId,
      nullptr,
      nullptr
    );
    resources->queues.push_back(this->queue);
  }

  void CommandQueue::push(
    cl_kernel op,
    std::vector<size_t> ranges,
    std::initializer_list<KernelArg> args
  ) {
    cl_int status;
    cl_uint i = 0;
    for (KernelArg arg: args) {
      status = clSetKernelArg(op, i, arg.size, arg.ptr);
      if (status != CL_SUCCESS) {
        std::cout << "clSetKernelArg failure: " << status << ", index: " << i << std::endl;
      }
      i++;
    }
    status = clEnqueueNDRangeKernel(
      queue,
      op,
      (cl_uint)ranges.size(),
      nullptr,
      &ranges[0],
      nullptr,
      0,
      nullptr,
      nullptr
    );
    if (status != CL_SUCCESS) {
      std::cout << "clEnqueueNDRangeKernel failure: " << status << std::endl;
      std::cout << "ranges: " << ranges[0] << std::endl;
    }
  }

  void CommandQueue::pushZeroFill(Memory *mem) {
    cl_int status;
    unsigned int zero = 0;
    status = clEnqueueFillBuffer(queue, mem->mem, &zero, sizeof(unsigned int), 0, sizeof(unsigned int) * mem->count, 0, nullptr, nullptr);
    if (status != CL_SUCCESS) {
      std::cout << "clEnqueueFillBuffer failure: " << status << std::endl;
    }
  }

  void CommandQueue::read(Memory *memory, void *hostPtr) {
    if (memory->count == 0) {
      return;
    }
    cl_int status;
    status = clEnqueueReadBuffer(
      queue,
      memory->mem,
      CL_TRUE,
      0,
      memory->size * memory->count,
      hostPtr,
      0,
      nullptr,
      nullptr
    );
    if (status != CL_SUCCESS) {
      std::cout << memory->name << " clEnqueueReadBuffer failure: " << status << std::endl;
    }
  }

  void CommandQueue::readRange(Memory *memory, void *hostPtrVoid, size_t min, size_t length) {
    if (length == 0) {
      return;
    }
    auto hostPtr = (char*)hostPtrVoid;
    cl_int status;
    status = clEnqueueReadBuffer(
      queue,
      memory->mem,
      CL_TRUE,
      memory->size * min,
      memory->size * length,
      hostPtr + (memory->size * min),
      0,
      nullptr,
      nullptr
    );
    if (status != CL_SUCCESS) {
      std::cout << memory->name << " clEnqueueReadBuffer failure: " << status << std::endl;
    }
  }

  void CommandQueue::write(Memory *memory) {
    if (memory->count == 0) {
      return;
    }
    cl_int status;
    status = clEnqueueWriteBuffer(
      queue,
      memory->mem,
      CL_TRUE,
      0,
      memory->size * memory->count,
      memory->hostPtr,
      0,
      nullptr,
      nullptr
    );
    if (status != CL_SUCCESS) {
      std::cout << memory->name << " clEnqueueWriteBuffer failure: " << status << std::endl;
    }
  }

  void CommandQueue::write(Memory *memory, size_t offset) {
    if (memory->count - offset <= 0) {
      return;
    }
    cl_int status;
    auto hostPtr = (char*)memory->hostPtr;
    status = clEnqueueWriteBuffer(
      queue,
      memory->mem,
      CL_TRUE,
      memory->size * offset,
      memory->size * (memory->count - offset),
      hostPtr + (memory->size * offset),
      0,
      nullptr,
      nullptr
    );
    if (status != CL_SUCCESS) {
      std::cout << memory->name << " clEnqueueWriteBuffer failure: " << status << std::endl;
    }
  }

  void CommandQueue::readAt(Memory *memory, void *hostPtr, unsigned int index) {
    cl_int status;
    status = clEnqueueReadBuffer(
      queue,
      memory->mem,
      CL_TRUE,
      memory->size * index,
      memory->size,
      hostPtr,
      0,
      nullptr,
      nullptr
    );
    if (status != CL_SUCCESS) {
      std::cout << memory->name << " clEnqueueReadBuffer failure: " << status << std::endl;
    }
  }
}