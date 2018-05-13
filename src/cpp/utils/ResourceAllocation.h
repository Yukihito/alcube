#ifndef ALCUBE_RESOURCEALLOCATION_H
#define ALCUBE_RESOURCEALLOCATION_H

#include "EventEmitter.h"
#include "MemoryPool.h"

namespace alcube::utils {
  class AllocationRange {
    public:
      EventEmitter onAllocationLengthChanged;
      void init(unsigned int minIndex, unsigned int length, MemoryPool<AllocationRange>* memoryPool);
      AllocationRange* allocate(unsigned int length);
      unsigned int getIndex();
      unsigned int getLength();
      unsigned int getAllocatedLength();

    private:
      unsigned int minIndex = 0;
      unsigned int length = 0;
      unsigned int allocatedLength = 0;
      MemoryPool<AllocationRange>* memoryPool = nullptr;
  };

  template <class T>
  class ResourceAllocation {
    public:
      T get();
      T* getPtr();
      void init(AllocationRange* range, T* values);

    private:
      AllocationRange* range;
      T* values;
  };

  template <class T>
  void ResourceAllocation<T>::init(alcube::utils::AllocationRange *range, T *values) {
    this->range = range;
    this->values = values;
  }

  template <class T>
  T ResourceAllocation<T>::get() {
    return values[range->getIndex()];
  }

  template <class T>
  T* ResourceAllocation<T>::getPtr() {
    return &values[range->getIndex()];
  }
}

#endif //ALCUBE_RESOURCEALLOCATION_H
