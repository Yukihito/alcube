#include "Actor.h"

namespace alcube::physics::softbody {
  Actor::Actor(): physics::Actor() {
    this->moveSubEventHandler.f = [&](utils::AllocationMoveEvent &e) {
      this->subEntities[e.dst] = this;
    };

    this->subDeallocationEventHandler.f = [&](utils::DeallocationEvent &e) {
      this->subAllocationRange->onMove.unsubscribe(this->moveSubEventHandler);
      this->subAllocationRange->onDeallocate.unsubscribe(this->subDeallocationEventHandler);
    };
  }

  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange,
    physics::Actor** actors,
    physics::softbody::Actor** subEntities
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange, actors);
    this->subAllocationRange->onMove.subscribe(moveSubEventHandler);
    this->subAllocationRange->onDeallocate.subscribe(subDeallocationEventHandler);
    this->subEntities = subEntities;
    this->subEntities[subAllocationRange->getIndex()] = this;
    subAllocation.init(subAllocationRange, gpuAccessor->dtos.hostSoftBodies);
    type.set(SOFT_BODY);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(float, elasticity, 1.0f);
    INIT_GPU_BASED_ARRAY_PROPERTY(unsigned int*, subAllocation, springIndices);
    INIT_GPU_BASED_ARRAY_PROPERTY(unsigned char*, subAllocation, springNodeIndices);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(unsigned int, springCount, 0);
    INIT_GPU_BASED_REFERENCE(subAllocation, actorIndex, allocationRange);
  }

  void Actor::addSpring(alcube::utils::AllocationRange *springAllocationRange, unsigned char nodeIndex) {
    auto springCount = this->springCount.get();
    springIndices.get()[springCount] = springAllocationRange->getIndex();
    springNodeIndices.get()[springCount] = nodeIndex;
    this->springCount.set(springCount + 1);
  }

  void Actor::beforeWrite() {}
}
