#include "Actor.h"

namespace alcube::physics::fluid {
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
    physics::fluid::Actor** subActors
  ) {
    physics::Actor::init(gpuAccessor, allocationRange, subAllocationRange, actors);
    this->subAllocationRange->onMove.subscribe(moveSubEventHandler);
    this->subAllocationRange->onDeallocate.subscribe(subDeallocationEventHandler);
    this->subActors = subActors;
    this->subActors[subAllocationRange->getIndex()] = this;
    this->subAllocation.init(subAllocationRange, gpuAccessor->dtos.hostFluids);
    this->gpuAccessor = gpuAccessor;
    type.set(FLUID);
    INIT_GPU_BASED_REFERENCE(gpu::dtos::Fluid, subAllocation, actorIndex, allocationRange);
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
