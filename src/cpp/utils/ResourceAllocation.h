#ifndef ALCUBE_RESOURCEALLOCATION_H
#define ALCUBE_RESOURCEALLOCATION_H

#include <unordered_set>
#include "EventEmitter.h"
#include "MemoryPool.h"

namespace alcube::utils {
  class AllocationRange {
    public:
      EventEmitter onBeforeMove;
      EventEmitter onAfterMove;
      EventEmitter onBeforeGc;
      void init(unsigned int minIndex, unsigned int length, MemoryPool<AllocationRange>* memoryPool);
      AllocationRange* allocate(unsigned int length);
      void deallocate();
      unsigned int getIndex();
      unsigned int getLength();
      unsigned int getAllocatedLength();
      void gc();
      bool isFree();
      void syncDeallocation(AllocationRange *other);

    private:
      AllocationRange* linkedAllocationRanges[4];
      unsigned int linkedAllocationRangeCount = 0;
      unsigned int minIndex = 0;
      unsigned int length = 0;
      unsigned int allocatedLength = 0;
      MemoryPool<AllocationRange>* memoryPool = nullptr;
      std::unordered_set<AllocationRange*> parts = {};
      bool isFreeInstance = true;
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
