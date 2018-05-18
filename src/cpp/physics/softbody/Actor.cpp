#include "Actor.h"

namespace alcube::physics::softbody {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange);
    subAllocationRange->syncDeallocation(allocationRange);
    this->hostActor.getPtr()->type = SOFT_BODY;
    this->hostSubState.init(subAllocationRange, gpuAccessor->dtos.hostSoftBodies);
    this->subState.init(subAllocationRange, gpuAccessor->dtos.softBodies);
    this->hostSubState.getPtr()->elasticity = 1.0f;
    updateIndex();
  }

  gpu::dtos::SoftBody* Actor::getSubState() {
    return hostSubState.getPtr();
  }

  void Actor::beforeWrite() {}

  void Actor::updateIndex() {
    this->hostActor.getPtr()->subIndex = (unsigned short)subAllocationRange->getIndex();
    this->hostSubState.getPtr()->actorIndex = (unsigned short)allocationRange->getIndex();
  }
}
