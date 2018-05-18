#include "ResourceAllocation.h"

namespace alcube::utils {
  void AllocationRange::init(unsigned int minIndex, unsigned int length, MemoryPool<AllocationRange>* memoryPool) {
    this->minIndex = minIndex;
    this->allocatedLength = 0;
    this->length = length;
    this->memoryPool = memoryPool;
    this->parts.clear();
    this->isFreeInstance = false;
    this->onBeforeMove.unbind();
    this->onAfterMove.unbind();
    this->onBeforeGc.unbind();
    this->linkedAllocationRangeCount = 0;
  }

  AllocationRange* AllocationRange::allocate(unsigned int length) {
    unsigned int nextMinIndex = minIndex + allocatedLength;
    allocatedLength += length;
    auto range = memoryPool->allocate();
    range->init(nextMinIndex, length, memoryPool);
    parts.emplace(range);
    return range;
  }

  void AllocationRange::deallocate() {
    isFreeInstance = true;
  }

  void AllocationRange::gc() {
    this->allocatedLength = 0;
    for (auto part: parts) {
      if (part->isFree()) {
        memoryPool->deallocate(part);
        parts.erase(part);
      } else {
        onBeforeMove.emit();
        unsigned int nextMinIndex = minIndex + allocatedLength;
        part->minIndex = nextMinIndex;
        onAfterMove.emit();
        part->gc();
        allocatedLength += part->length;
      }
    }
  }

  bool AllocationRange::isFree() {
    for (auto &linkedAllocationRange : linkedAllocationRanges) {
      if (!linkedAllocationRange->isFree()) {
        return false;
      }
    }
    return isFreeInstance;
  }

  void AllocationRange::syncDeallocation(alcube::utils::AllocationRange *other) {
    linkedAllocationRanges[linkedAllocationRangeCount] = other;
    linkedAllocationRangeCount++;
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
