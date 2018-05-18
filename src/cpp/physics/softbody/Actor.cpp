#include "Actor.h"

namespace alcube::physics::softbody {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange);
    type.set(SOFT_BODY);
    subStateStruct.init(gpuAccessor->dtos.hostSoftBodies, gpuAccessor->dtos.softBodies, subAllocationRange);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(elasticity, 1.0f);
    INIT_GPU_BASED_SOFTBODY_ARRAY(springIndices);
    INIT_GPU_BASED_SOFTBODY_ARRAY(springNodeIndices);
    INIT_GPU_BASED_SOFTBODY_PROPERTY(springCount, 0);
    INIT_GPU_BASED_REFERENCE(gpu::dtos::SoftBody, subStateStruct, actorIndex, allocationRange);
    updateIndex();
  }

  void Actor::beforeWrite() {}

  void Actor::updateIndex() {
  }
}
