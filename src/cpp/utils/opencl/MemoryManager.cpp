#include "MemoryManager.h"

namespace alcube::utils::opencl {
  MemoryManager::MemoryManager(Resources *resources) {
    this->resources = resources;
  }

  Memory* MemoryManager::define(
    const std::string &name,
    size_t size,
    void *hostPtr,
    cl_mem_flags memFlags,
    size_t allocationCount
  ) {
    auto mem = new Memory(
      name,
      size,
      resources->context,
      hostPtr,
      memFlags,
      allocationCount
    );
    memories.push_back(mem);
    return mem;
  }

  void MemoryManager::allocate() {
    for (Memory* mem: memories) {
      mem->allocate();
    }
  }

  void MemoryManager::release() {
    for (Memory* mem: memories) {
      mem->release();
    }
  }
}