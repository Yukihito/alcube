#include "Actor.h"

namespace alcube::physics {
  void Actor::init(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::utils::AllocationRange *allocationRange
  ) {
    this->allocationRange = allocationRange;
    this->actor.init(allocationRange, gpuAccessor->dtos.actors);
    this->hostPhysicalQuantity.init(allocationRange, gpuAccessor->dtos.hostPhysicalQuantities);
    this->actorState.init(allocationRange, gpuAccessor->dtos.actorStates);
    this->physicalQuantity.init(allocationRange, gpuAccessor->dtos.physicalQuantities);
    this->hostPhysicalQuantity.getPtr()->radius = 1.0f;
    this->hostPhysicalQuantity.getPtr()->mass = 1.0f;
    this->hostPhysicalQuantity.getPtr()->rotation = {0.0f, 0.0f, 0.0f, 1.0f};
    this->hostPhysicalQuantity.getPtr()->angularMomentum = {0.0f, 0.0f, 0.0f};
    this->hostPhysicalQuantity.getPtr()->linearMomentum = {0.0f, 0.0f, 0.0f};
    this->hostPhysicalQuantity.getPtr()->position = {0.0f, 0.0f, 0.0f};
  }

  unsigned short Actor::getSubIndex() {
    return actor.getPtr()->subPhysicalQuantityIndex;
  }

  gpu::dtos::PhysicalQuantity* Actor::getPhysicalQuantityDto() {
    return this->hostPhysicalQuantity.getPtr();
  }
}