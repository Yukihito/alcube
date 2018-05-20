#ifndef ALCUBE_GPUBASEDPROPERTY_H
#define ALCUBE_GPUBASEDPROPERTY_H

#include "ResourceAllocation.h"

namespace alcube::utils {
  template <class T, class U>
  class GPUBasedProperty {
    public:
      explicit GPUBasedProperty() = default;

      void init(ResourceAllocation<T>& owner, size_t position, std::function<U()> getter, std::function<void(U)> setter) {
        this->getter = std::move(getter);
        this->setter = std::move(setter);
        this->owner = &owner;
      };

      U get() {
        return getter();
      };

      void set(U arg) {
        setter(arg);
      };

    protected:
      ResourceAllocation<T>* owner = nullptr;
      std::function<U()> getter = {};
      std::function<void(U)> setter = {};
  };

  template <class T>
  class GPUBasedReference : public GPUBasedProperty<T, unsigned int> {
    public:
      explicit GPUBasedReference() {
        moveReferenceEventHandler.f = [&](AllocationMoveEvent &e) {
          this->value = e.dst;
        };

        deallocationEventHandler.f = [&](DeallocationEvent &e) {
          this->referenceAllocationRange->onMove.unsubscribe(this->moveReferenceEventHandler);
          this->owner->range->onDeallocate.unsubscribe(this->deallocationEventHandler);
        };
      }

      void init(ResourceAllocation<T>& owner, size_t position, AllocationRange* referenceAllocationRange, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter) {
        GPUBasedProperty<T, unsigned int>::init(owner, position, getter, setter);
        this->referenceAllocationRange = referenceAllocationRange;
        this->owner->range->onDeallocate.subscribe(deallocationEventHandler);
        this->referenceAllocationRange->onMove.subscribe(moveReferenceEventHandler);
        this->value = referenceAllocationRange->getIndex();
        this->set(value);
      }

      void update() {
        this->set(value);
      }

    private:
      unsigned int value = 0;
      AllocationRange* referenceAllocationRange = nullptr;
      EventHandler<AllocationMoveEvent> moveReferenceEventHandler = {};
      EventHandler<DeallocationEvent> deallocationEventHandler = {};
  };
}

#define INIT_GPU_BASED_PROPERTY(T, argOwner, argName) { this->argName.init(argOwner, 0, [&]{return (argOwner).getPtr()->argName;}, [&](T arg){(argOwner).getPtr()->argName = arg;}); }
#define INIT_GPU_BASED_ARRAY_PROPERTY(T, argOwner, argName) { this->argName.init(argOwner, 0, [&]{return (argOwner).getPtr()->argName;}, [&](T arg){}); }
#define INIT_GPU_BASED_REFERENCE(T, argOwner, argName, referenceAllocationRange) { this->argName.init(argOwner, 0, referenceAllocationRange, [&]{return (argOwner).getPtr()->argName;}, [&](unsigned int arg){(argOwner).getPtr()->argName = arg;}); }
#define INIT_GPU_BASED_REFERENCE_AT(T, argOwner, argName, referenceAllocationRange, index) { this->argName.init(argOwner, 0, referenceAllocationRange, [&]{return (argOwner).getPtr()->argName[index];}, [&](unsigned int arg){(argOwner).getPtr()->argName[index] = arg;}); }

#endif //ALCUBE_GPUBASEDPROPERTY_H
