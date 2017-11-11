#include "Memory.h"

namespace alcube::utils::opencl {

  Memory::Memory(
    const std::string& name,
    size_t size,
    cl_context& context,
    void* hostPtr,
    bool isReadable,
    bool isWritable
  ) {
    this->hostPtr = hostPtr;
    this->name = name;
    this->size = size;
    this->context = context;
    this->count = 1;

    if (isReadable && !isWritable) {
      this->memFlags = CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR;
    } else if (!isReadable && isWritable) {
      this->memFlags = CL_MEM_WRITE_ONLY;
    } else if (isReadable) { // && isWritable
      this->memFlags = CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR;
    } else {
      this->memFlags = 0;
    }
  }

  void Memory::allocate() {
    cl_int status;
    size_t allocationCount = count;
    if (allocationCount == 0) {
      allocationCount = 1;
    }
    mem = clCreateBuffer(
      context,
      memFlags,
      size * allocationCount,
      hostPtr,
      &status
    );
    if (status != CL_SUCCESS) {
      std::cout << name << " clCreateBuffer failure: " << status << std::endl;
    }
  }

  void Memory::release() {
    clReleaseMemObject(mem);
  }
}