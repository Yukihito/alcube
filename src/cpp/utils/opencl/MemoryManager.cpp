#include "MemoryManager.h"

namespace alcube::utils::opencl {
  MemoryManager::MemoryManager(Resources *resources) {
    this->resources = resources;
  }

  Memory* MemoryManager::define(
    const std::string &name,
    size_t size,
    void *hostPtr,
    bool isReadable,
    bool isWritable
  ) {
    auto mem = new Memory(
      name,
      size,
      resources->context,
      hostPtr,
      isReadable,
      isWritable
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