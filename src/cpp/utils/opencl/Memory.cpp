#include "Memory.h"

namespace alcube::utils::opencl {

  Memory::Memory(
    const std::string& name,
    size_t size,
    cl_context& context,
    void* hostPtr,
    cl_mem_flags memFlags
  ) {
    this->hostPtr = hostPtr;
    this->name = name;
    this->size = size;
    this->context = context;
    this->count = 1;
    this->memFlags = memFlags;
    this->allocationCount = 1;
  }

  void Memory::allocate() {
    cl_int status;
    if (allocationCount == 0) {
      return;
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
    if (mem != nullptr) {
      clReleaseMemObject(mem);
    }
  }
}