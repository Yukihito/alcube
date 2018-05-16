#ifndef ALCUBE_GPUBASEDPROPERTY_H
#define ALCUBE_GPUBASEDPROPERTY_H

#include "ResourceAllocation.h"

namespace alcube::utils {
  template <class T>
  class GPUBasedStruct {
    public:
      void init(T* hostPtr, T* gpuPtr, AllocationRange* allocationRange) {
        inputResourceAllocation.init(allocationRange, hostPtr);
        outputResourceAllocation.init(allocationRange, gpuPtr);
      };

      T* get() {
        return outputResourceAllocation.getPtr();
      };

      void set(T* arg) {
        inputResourceAllocation.getPtr()[0] = arg;
        outputResourceAllocation.getPtr()[0] = arg;
      };

    private:
      AllocationRange* allocationRange;
      ResourceAllocation<T> inputResourceAllocation;
      ResourceAllocation<T> outputResourceAllocation;
  };

  template <class T, class U>
  class GPUBasedProperty {
    public:
      void init(GPUBasedStruct<T>* owner, size_t position) {
        this->owner = owner;
        this->position = position;
      };

      U get() {
        auto ptr = (U*)(owner->get() + position);
        return *ptr;
      };

      void set(U arg) {
        auto ptr = (U*)(owner->get() + position);
        ptr[0] = arg;
      };

    private:
      GPUBasedStruct<T>* owner;
      size_t position;
  };

  template <class T>
  class GPUBasedReference : public GPUBasedProperty<T, unsigned int> {
    public:
      void init(GPUBasedStruct<T>* owner, size_t position, AllocationRange* referenceAllocationRange) {
        GPUBasedProperty<T, unsigned int>::init(owner, position);
        this->referenceAllocationRange = referenceAllocationRange;
        referenceAllocationRange->onAfterMove.subscribe([&]{
          this->set(referenceAllocationRange->getIndex());
        });
      }
    private:
      AllocationRange* referenceAllocationRange = nullptr;
  };
}

#define INIT_GPU_BASED_PROPERTY(argOwner, argName) { T tmp; this->argName.init(argOwner, &tmp.argName - &tmp); }
#define INIT_GPU_BASED_REFERENCE(argOwner, argName, referenceAllocationRange) { T tmp; this->argName.init(argOwner, &tmp.argName - &tmp, referenceAllocationRange); }

#endif //ALCUBE_GPUBASEDPROPERTY_H
