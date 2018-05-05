#include "Actor.h"

namespace alcube::physics::softbody {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ) {
    physics::Actor::init(gpuAccessor, allocationRange);
    this->subAllocationRange = subAllocationRange;
    this->actor.getPtr()->type = SOFT_BODY;
    this->hostSubPhysicalQuantity.init(subAllocationRange, gpuAccessor->dtos.hostSoftBodies);
    this->subPhysicalQuantity.init(subAllocationRange, gpuAccessor->dtos.softBodies);
    this->hostSubPhysicalQuantity.getPtr()->elasticity = 1.0f;

    updateIndex();
  }

  gpu::dtos::SoftBody* Actor::getSubPhysicalQuantity() {
    return hostSubPhysicalQuantity.getPtr();
  }

  unsigned short Actor::getIndex() {
    return hostSubPhysicalQuantity.getPtr()->actorIndex;
  }

  void Actor::beforeWrite() {}

  void Actor::updateIndex() {
    this->actor.getPtr()->subPhysicalQuantityIndex = (unsigned short)subAllocationRange->getIndex();
    this->hostSubPhysicalQuantity.getPtr()->actorIndex = (unsigned short)allocationRange->getIndex();
  }
}
