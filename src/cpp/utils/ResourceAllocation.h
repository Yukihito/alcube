#ifndef ALCUBE_RESOURCEALLOCATION_H
#define ALCUBE_RESOURCEALLOCATION_H

#include <vector>
#include "EventEmitter.h"
#include "MemoryPool.h"

namespace alcube::utils {
  struct AllocationMoveEvent {
    unsigned int src = 0;
    unsigned int dst = 0;
  };

  struct DeallocationEvent {};

  class AllocationRange {
    public:
      EventEmitter<AllocationMoveEvent> onMove = {};
      EventEmitter<DeallocationEvent> onDeallocate = {};

      void init(unsigned int minIndex, unsigned int length, MemoryPool<AllocationRange>* memoryPool);
      AllocationRange* allocate(unsigned int length);
      void deallocate();
      void deallocateOn(std::function<bool()> cond);
      unsigned int getIndex();
      unsigned int getLength();
      unsigned int getAllocatedLength();
      void gc();
      bool isFree();

    private:
      unsigned int minIndex = 0;
      unsigned int length = 0;
      unsigned int allocatedLength = 0;
      MemoryPool<AllocationRange>* memoryPool = nullptr;
      std::vector<AllocationRange*> parts = {};
      bool isFreeInstance = true;
      bool isSpecifiedDeallocateCondition = false;
      std::function<bool()> deallocateCondition = {};
      unsigned int partsCount = 0;
  };

  template <class T>
  class ResourceAllocation {
    public:
      AllocationRange* range = nullptr;
      explicit ResourceAllocation() {
        this->moveEventHandler.f = [&](AllocationMoveEvent &e){
          this->values[e.dst] = this->values[e.src];
        };
        this->deallocationEventHandler.f = [&](DeallocationEvent &e) {
          this->range->onMove.unsubscribe(this->moveEventHandler);
          this->range->onDeallocate.unsubscribe(this->deallocationEventHandler);
        };
      };
      T get();
      T* getPtr();
      void init(AllocationRange* range, T* values);
    private:
      T* values = nullptr;
      EventHandler<AllocationMoveEvent> moveEventHandler = {};
      EventHandler<DeallocationEvent> deallocationEventHandler = {};
  };

  template <class T>
  void ResourceAllocation<T>::init(alcube::utils::AllocationRange *range, T *values) {
    this->range = range;
    this->values = values;
    range->onMove.subscribe(moveEventHandler);
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
