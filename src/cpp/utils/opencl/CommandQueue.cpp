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

  CommandQueue::CommandQueue(Resources *resources) {
    this->resources = resources;
    this->queue = clCreateCommandQueue(
      this->resources->context,
      this->resources->deviceId,
      0,
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
      status = clSetKernelArg(op, i, sizeof(cl_mem), arg.ptr);
      if (status != CL_SUCCESS) {
        std::cout << "clSetKernelArg failure: " << status << std::endl;
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
    }
  }

  void CommandQueue::read(Memory *memory, void *hostPtr) {
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
}