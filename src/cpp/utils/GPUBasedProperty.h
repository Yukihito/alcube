#ifndef ALCUBE_GPUBASEDPROPERTY_H
#define ALCUBE_GPUBASEDPROPERTY_H

#include "ResourceAllocation.h"

namespace alcube::utils {
  template <class T, class U>
  class GPUBasedProperty {
    public:
      explicit GPUBasedProperty() = default;

      void init(ResourceAllocation<T>& owner, std::function<U()> getter, std::function<void(U)> setter) {
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
        moveReferenceEventHandler.f = [this](AllocationMoveEvent &e) {
          this->set(e.dst);
        };

        deallocationEventHandler.f = [this](DeallocationEvent &e) {
          this->referenceAllocationRange->onMove.unsubscribe(this->moveReferenceEventHandler);
          this->owner->range->onDeallocate.unsubscribe(this->deallocationEventHandler);
        };
      }

      void init(ResourceAllocation<T>& owner, AllocationRange* referenceAllocationRange, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter) {
        GPUBasedProperty<T, unsigned int>::init(owner, getter, setter);
        this->referenceAllocationRange = referenceAllocationRange;
        this->owner->range->onDeallocate.subscribe(deallocationEventHandler);
        this->referenceAllocationRange->onMove.subscribe(moveReferenceEventHandler);
        this->set(referenceAllocationRange->getIndex());
      }

    private:
      AllocationRange* referenceAllocationRange = nullptr;
      EventHandler<AllocationMoveEvent> moveReferenceEventHandler = {};
      EventHandler<DeallocationEvent> deallocationEventHandler = {};
  };
}

#define INIT_GPU_BASED_PROPERTY(T, argOwner, argName) { this->argName.init(argOwner, [this]{ return this->argOwner.getPtr()->argName; }, [this](T arg){ this->argOwner.getPtr()->argName = arg; }); }
#define INIT_GPU_BASED_ARRAY_PROPERTY(T, argOwner, argName) { this->argName.init(argOwner, [this]{ return this->argOwner.getPtr()->argName; }, [](T arg){}); }
#define INIT_GPU_BASED_REFERENCE(argOwner, argName, referenceAllocationRange) { this->argName.init(argOwner, referenceAllocationRange, [this]{return this->argOwner.getPtr()->argName;}, [this](unsigned int arg){ this->argOwner.getPtr()->argName = arg;}); }

#endif //ALCUBE_GPUBASEDPROPERTY_H
