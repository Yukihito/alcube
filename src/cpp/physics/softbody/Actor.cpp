#include "Actor.h"

namespace alcube::physics::softbody {
  Actor::Actor(): physics::Actor() {
    this->moveSubEventHandler.f = [&](utils::AllocationMoveEvent &e) {
      this->subActors[e.dst] = this;
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
    physics::softbody::Actor** subActors
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange, actors);
    this->subAllocationRange->onMove.subscribe(moveSubEventHandler);
    this->subAllocationRange->onDeallocate.subscribe(subDeallocationEventHandler);
    this->subActors = subActors;
    this->subActors[subAllocationRange->getIndex()] = this;
    subAllocation.init(subAllocationRange, gpuAccessor->dtos.hostSoftBodies);
    type.set(SOFT_BODY);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(float, elasticity, 1.0f);
    INIT_GPU_BASED_ARRAY_PROPERTY(unsigned int*, subAllocation, springIndices);
    INIT_GPU_BASED_ARRAY_PROPERTY(unsigned char*, subAllocation, springNodeIndices);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(unsigned int, springCount, 0);
    INIT_GPU_BASED_REFERENCE(gpu::dtos::SoftBody, subAllocation, actorIndex, allocationRange);
  }

  void Actor::beforeWrite() {}
}
