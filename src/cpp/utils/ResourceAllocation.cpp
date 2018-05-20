#include "ResourceAllocation.h"
#include <utility>

namespace alcube::utils {
  void AllocationRange::init(unsigned int minIndex, unsigned int length, MemoryPool<AllocationRange>* memoryPool) {
    this->minIndex = minIndex;
    this->allocatedLength = 0;
    this->length = length;
    this->memoryPool = memoryPool;
    this->parts.clear();
    this->isFreeInstance = false;
    this->onDeallocate.unbind();
    this->onMove.unbind();
    this->isSpecifiedDeallocateCondition = false;
    this->partsCount = 0;
  }

  AllocationRange* AllocationRange::allocate(unsigned int length) {
    unsigned int nextMinIndex = minIndex + allocatedLength;
    allocatedLength += length;
    auto range = memoryPool->allocate();
    range->init(nextMinIndex, length, memoryPool);
    parts.push_back(range);
    partsCount++;
    return range;
  }

  void AllocationRange::deallocate() {
    isFreeInstance = true;
  }

  void AllocationRange::deallocateOn(std::function<bool()> cond) {
    this->isSpecifiedDeallocateCondition = true;
    this->deallocateCondition = std::move(cond);
  }

  void AllocationRange::gc() {
    this->allocatedLength = 0;
    unsigned int nextPartsCount = 0;
    for (unsigned int i = 0; i < partsCount; i++) {
      auto part = parts[i];
      if (part->isFree()) {
        DeallocationEvent e;
        part->onDeallocate.emit(e);
        memoryPool->deallocate(part);
      } else {
        unsigned int nextMinIndex = minIndex + allocatedLength;
        part->minIndex = nextMinIndex;
        part->gc();
        allocatedLength += part->length;
        parts[nextPartsCount] = part;
        AllocationMoveEvent e;
        e.dst = nextMinIndex;
        e.src = i;
        part->onMove.emit(e);
        nextPartsCount++;
      }
    }
    partsCount = nextPartsCount;
  }

  bool AllocationRange::isFree() {
    if (isSpecifiedDeallocateCondition) {
      isFreeInstance = deallocateCondition();
    }
    return isFreeInstance;
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
