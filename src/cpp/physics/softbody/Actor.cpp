#include "Actor.h"

namespace alcube::physics::softbody {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ) {
    physics::Actor::init(gpuAccessor, allocationRange);
    subAllocationRange->syncDeallocation(allocationRange);
    this->subAllocationRange = subAllocationRange;
    this->hostActor.getPtr()->type = SOFT_BODY;
    this->hostSubState.init(subAllocationRange, gpuAccessor->dtos.hostSoftBodies);
    this->subState.init(subAllocationRange, gpuAccessor->dtos.softBodies);
    this->hostSubState.getPtr()->elasticity = 1.0f;

    updateIndex();
  }

  gpu::dtos::SoftBody* Actor::getSubState() {
    return hostSubState.getPtr();
  }

  unsigned short Actor::getIndex() {
    return hostSubState.getPtr()->actorIndex;
  }

  void Actor::beforeWrite() {}

  void Actor::updateIndex() {
    this->hostActor.getPtr()->subPhysicalQuantityIndex = (unsigned short)subAllocationRange->getIndex();
    this->hostSubState.getPtr()->actorIndex = (unsigned short)allocationRange->getIndex();
  }
}
