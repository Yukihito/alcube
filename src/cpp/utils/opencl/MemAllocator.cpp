#include "MemAllocator.h"

namespace alcube::utils::opencl {

  MemAllocator::MemAllocator(
    const std::string& name,
    size_t size,
    void* ptr,
    cl_context& context,
    cl_command_queue& queue
  ) {
    this->ptr = ptr;
    this->name = name;
    this->size = size;
    this->context = context;
    this->count = 1;
    this->queue = queue;
  }

  void MemAllocator::release() {
    clReleaseMemObject(mem);
  }

  ReadableMemAllocator::ReadableMemAllocator(
    const std::string& name,
    size_t size,
    void *ptr,
    cl_context& context,
    cl_command_queue& queue
  ) : MemAllocator(name, size, ptr, context, queue) {}

  void ReadableMemAllocator::allocate() {
    cl_int status;
    size_t allocationCount = count;
    if (allocationCount == 0) {
      allocationCount = 1;
    }
    mem = clCreateBuffer(
      context,
      CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
      size * allocationCount,
      ptr,
      &status
    );
    if (status != CL_SUCCESS) {
      std::cout << name << " clCreateBuffer failure: " << status << std::endl;
    }
  }

  void ReadableMemAllocator::read() {
  }

  WritableMemAllocator::WritableMemAllocator(
    const std::string& name,
    size_t size,
    void *ptr,
    cl_context& context,
    cl_command_queue& queue
  ) : MemAllocator(name, size, ptr, context, queue) {}

  void WritableMemAllocator::allocate() {
    cl_int status;
    mem = clCreateBuffer(
      context,
      CL_MEM_WRITE_ONLY,
      size * count,
      nullptr,
      &status
    );
    if (status != CL_SUCCESS) {
      std::cout << name << " clCreateBuffer failure: " << status << std::endl;
    }
  }

  void WritableMemAllocator::read() {
    cl_int status;
    status = clEnqueueReadBuffer(queue, mem, CL_TRUE, 0, size * count, ptr, 0, nullptr, nullptr);
    if (status != CL_SUCCESS) {
      std::cout << name << " clEnqueueReadBuffer failure: " << status << std::endl;
    }
  }
}