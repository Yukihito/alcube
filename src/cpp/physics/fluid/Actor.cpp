#include "Actor.h"

namespace alcube::physics::fluid {
  void Actor::init(
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    utils::AllocationRange* subAllocationRange
  ){
    physics::Actor::init(gpuAccessor, allocationRange);
    this->subAllocationRange = subAllocationRange;
    this->actor->getPtr()->type = FLUID;
    this->hostSubPhysicalQuantity = new utils::ResourceAllocation<gpu::dtos::Fluid>(subAllocationRange, gpuAccessor->dtos.hostFluids);
    this->subPhysicalQuantity = new utils::ResourceAllocation<gpu::dtos::Fluid>(subAllocationRange, gpuAccessor->dtos.fluids);

    updateIndex();
    Actor::instances[this]= this;
  }

  gpu::dtos::Fluid* Actor::getSubPhysicalQuantity() {
    return this->hostSubPhysicalQuantity->getPtr();
  }

  unsigned short Actor::getIndex() {
    return hostSubPhysicalQuantity->getPtr()->actorIndex;
  }

  void Actor::updateIndex() {
    this->actor->getPtr()->subPhysicalQuantityIndex = (unsigned short)subAllocationRange->getIndex();
    this->hostSubPhysicalQuantity->getPtr()->actorIndex = (unsigned short)allocationRange->getIndex();
  }

  std::map<physics::Actor*, Actor*> Actor::instances = {};
  float Actor::stiffness = 64.0f;
  float Actor::density = 0.02f;
  float Actor::viscosity = 8.0f;
  float Actor::mass = (4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * Actor::density;
}