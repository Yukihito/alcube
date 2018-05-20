#include "Actor.h"

namespace alcube::physics::softbody {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange,
    physics::Actor** actors,
    physics::softbody::Actor** subEntities
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange, actors);
    this->subAllocation.init(subAllocationRange, gpuAccessor->dtos.hostSoftBodies);
    this->subEntityAllocation.init(subAllocationRange, subEntities);
    this->subEntityAllocation.set(this);

    type.set(SOFT_BODY);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(float, elasticity, 1.0f);
    INIT_GPU_BASED_ARRAY_PROPERTY(unsigned char*, subAllocation, springNodeIndices);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(unsigned int, springCount, 0);
    INIT_GPU_BASED_REFERENCE(subAllocation, actorIndex, allocationRange);
    actorIndex.enableLifeSync();
  }

  void Actor::addSpring(alcube::utils::AllocationRange *springAllocationRange, unsigned char nodeIndex) {
    auto springCount = this->springCount.get();
    this->springIndices[springCount].init(
      subAllocation,
      springAllocationRange,
      [this, springCount]{
        return this->subAllocation.getPtr()->springIndices[springCount];
      },
      [this, springCount](unsigned int arg){
        this->subAllocation.getPtr()->springIndices[springCount] = arg;
      }
    );
    springNodeIndices.get()[springCount] = nodeIndex;
    this->springCount.set(springCount + 1);
  }

  void Actor::beforeWrite() {}
}
