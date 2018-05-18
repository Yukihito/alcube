#ifndef ALCUBE_GPUBASEDPROPERTY_H
#define ALCUBE_GPUBASEDPROPERTY_H

#include "ResourceAllocation.h"

namespace alcube::utils {
  template <class T>
  class GPUBasedStruct {
    public:
      void init(T* hostPtr, T* gpuPtr, AllocationRange* allocationRange) {
        this->allocationRange = allocationRange;
        inputResourceAllocation.init(allocationRange, hostPtr);
        outputResourceAllocation.init(allocationRange, gpuPtr);
        beforeMove.f = [&]{
          this->temporal = *this->getOutputPtr();
        };
        afterMove.f = [&]{
          this->getInputPtr()[0] = temporal;
        };
        allocationRange->onBeforeMove.subscribe(&beforeMove);
        allocationRange->onAfterMove.subscribe(&afterMove);
      };

      T* getOutputPtr() {
        return outputResourceAllocation.getPtr();
      };

      T* getInputPtr() {
        return inputResourceAllocation.getPtr();
      }

      bool isFree() {
        return allocationRange->isFree();
      }

    private:
      AllocationRange* allocationRange = nullptr;
      T temporal = {};
      ResourceAllocation<T> inputResourceAllocation = {};
      ResourceAllocation<T> outputResourceAllocation = {};
      EventHandler beforeMove = {};
      EventHandler afterMove = {};
  };

  template <class T, class U>
  class GPUBasedProperty {
    public:
      void init(GPUBasedStruct<T>& owner, size_t position) {
        this->owner = &owner;
        this->position = position;
      };

      U get() {
        auto ptr = (U*)((char*)owner->getInputPtr() + position);
        return *ptr;
      };

      void set(U arg) {
        auto ptr = (U*)((char*)owner->getInputPtr() + position);
        ptr[0] = arg;
      };

    protected:
      GPUBasedStruct<T>* owner;
    private:
      size_t position;
  };

  template <class T>
  class GPUBasedReference : public GPUBasedProperty<T, unsigned int> {
    public:
      void init(GPUBasedStruct<T>& owner, size_t position, AllocationRange* referenceAllocationRange) {
        GPUBasedProperty<T, unsigned int>::init(owner, position);
        this->referenceAllocationRange = referenceAllocationRange;
        afterMove.f = [&]{
          if (this->owner->isFree()) {
            this->referenceAllocationRange->onAfterMove.unsubscribe(&afterMove);
          } else {
            this->set(referenceAllocationRange->getIndex());
          }
        };
        this->referenceAllocationRange->onAfterMove.subscribe(&afterMove);
      }
    private:
      AllocationRange* referenceAllocationRange = nullptr;
      EventHandler afterMove = {};
  };
}

#define INIT_GPU_BASED_PROPERTY(T, argOwner, argName) { T tmp = {}; this->argName.init(argOwner, (char*)&tmp.argName - (char*)&tmp); }
#define INIT_GPU_BASED_REFERENCE(T, argOwner, argName, referenceAllocationRange) { T tmp = {}; this->argName.init(argOwner, (char*)&tmp.argName - (char*)&tmp, referenceAllocationRange); }

#endif //ALCUBE_GPUBASEDPROPERTY_H
