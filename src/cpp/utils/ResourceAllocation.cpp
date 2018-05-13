#include "ResourceAllocation.h"

namespace alcube::utils {
  void AllocationRange::init(unsigned int minIndex, unsigned int length, MemoryPool<AllocationRange>* memoryPool) {
    this->minIndex = minIndex;
    this->allocatedLength = 0;
    this->length = length;
    this->memoryPool = memoryPool;
  }

  AllocationRange* AllocationRange::allocate(unsigned int length) {
    unsigned int nextMinIndex = minIndex + allocatedLength;
    allocatedLength += length;
    onAllocationLengthChanged.emit();
    auto range = memoryPool->get();
    range->init(nextMinIndex, length, memoryPool);
    return range;
  }

  unsigned int AllocationRange::getIndex() {
    return minIndex;
  }

  unsigned int AllocationRange::getLength() {
    return length;
  }

  unsigned int AllocationRange::getAllocatedLength() {
    return allocatedLength;
  }
}
