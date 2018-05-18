#include "Actor.h"

namespace alcube::physics::fluid {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange);
    subAllocationRange->syncDeallocation(allocationRange);
    type.set(FLUID);
    this->hostSubState.init(subAllocationRange, gpuAccessor->dtos.hostFluids);
    this->subState.init(subAllocationRange, gpuAccessor->dtos.fluids);
    this->gpuAccessor = gpuAccessor;
    updateIndex();
  }

  gpu::dtos::Fluid* Actor::getSubState() {
    return this->hostSubState.getPtr();
  }

  void Actor::beforeWrite() {
    radius.set(gpuAccessor->memories.fluidSettings.at(0)->effectiveRadius / 2.0f);
    physics::Actor::mass.set(gpuAccessor->memories.fluidSettings.at(0)->particleMass);
  }

  void Actor::updateIndex() {
    this->hostSubState.getPtr()->actorIndex = (unsigned short)allocationRange->getIndex();
  }

  float Actor::stiffness = 64.0f;
  float Actor::density = 0.02f;
  float Actor::viscosity = 8.0f;
  float Actor::mass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * Actor::density;
}
