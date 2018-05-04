#include "ResourceAllocation.h"

namespace alcube::utils {
  AllocationRange::AllocationRange(unsigned int minIndex, unsigned int length) {
    this->minIndex = minIndex;
    this->allocatedLength = 0;
    this->length = length;
  }

  AllocationRange* AllocationRange::allocate(unsigned int length) {
    unsigned int nextMinIndex = minIndex + allocatedLength;
    allocatedLength += length;
    onAllocationLengthChanged.emit();
    return new AllocationRange(nextMinIndex, length);
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
