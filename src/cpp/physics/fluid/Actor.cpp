#include "Actor.h"

namespace alcube::physics::fluid {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ){
    physics::Actor::init(gpuAccessor, allocationRange);
    subAllocationRange->syncDeallocation(allocationRange);
    this->subAllocationRange = subAllocationRange;
    this->hostActor.getPtr()->type = FLUID;
    this->hostSubState.init(subAllocationRange, gpuAccessor->dtos.hostFluids);
    this->subState.init(subAllocationRange, gpuAccessor->dtos.fluids);
    this->gpuAccessor = gpuAccessor;
    updateIndex();
  }

  gpu::dtos::Fluid* Actor::getSubPhysicalQuantity() {
    return this->hostSubState.getPtr();
  }

  unsigned short Actor::getIndex() {
    return hostSubState.getPtr()->actorIndex;
  }

  void Actor::beforeWrite() {
    getDto()->radius = gpuAccessor->memories.fluidSettings.at(0)->effectiveRadius / 2.0f;
    getDto()->mass = gpuAccessor->memories.fluidSettings.at(0)->particleMass;
  }

  void Actor::updateIndex() {
    this->hostActor.getPtr()->subPhysicalQuantityIndex = (unsigned short)subAllocationRange->getIndex();
    this->hostSubState.getPtr()->actorIndex = (unsigned short)allocationRange->getIndex();
  }

  float Actor::stiffness = 64.0f;
  float Actor::density = 0.02f;
  float Actor::viscosity = 8.0f;
  float Actor::mass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * Actor::density;
}
