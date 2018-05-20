#include "Actor.h"

namespace alcube::physics::fluid {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange,
    physics::Actor** actors,
    physics::fluid::Actor** subEntities
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange, actors);
    this->subAllocation.init(subAllocationRange, gpuAccessor->dtos.hostFluids);
    this->subEntityAllocation.init(subAllocationRange, subEntities);
    this->subEntityAllocation.set(this);
    this->gpuAccessor = gpuAccessor;
    type.set(FLUID);
    INIT_GPU_BASED_REFERENCE(subAllocation, actorIndex, allocationRange);
  }

  void Actor::beforeWrite() {
    radius.set(gpuAccessor->memories.fluidSettings.at(0)->effectiveRadius / 2.0f);
    physics::Actor::mass.set(gpuAccessor->memories.fluidSettings.at(0)->particleMass);
  }

  float Actor::stiffness = 64.0f;
  float Actor::density = 0.02f;
  float Actor::viscosity = 8.0f;
  float Actor::mass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * Actor::density;
}
