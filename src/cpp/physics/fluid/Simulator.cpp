#include "Simulator.h"

namespace alcube::physics::fluid {
  Simulator::Simulator(gpu::GPUAccessor *gpuAccessor, ActorResources* actorResources) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->actorResources = actorResources;
    activeActorCount = 0;
    allActorCount = 0;
  }

  void Simulator::setUpConstants() {
    auto fluidSettings = memories.fluidSettings.at(0);
    fluidSettings->stiffness = Actor::stiffness;
    fluidSettings->density = Actor::density;
    fluidSettings->viscosity = Actor::viscosity;
    fluidSettings->particleMass = Actor::mass;
    fluidSettings->effectiveRadius = 2.0f;
    fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(fluidSettings->effectiveRadius, 9));
    fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
    fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
  }

  void Simulator::input() {
    allActorCount = actorResources->fluidResource->allocationRange->getAllocatedLength();
    auto updateCount = allActorCount - activeActorCount;
    if (updateCount == 0) {
      return;
    }
    memories.hostFluids.setCount(allActorCount);
    memories.fluids.setCount(allActorCount);
    memories.hostFluids.write(activeActorCount);
    kernels.inputFluids(
      updateCount,
      memories.hostFluids,
      memories.fluids,
      (unsigned short)activeActorCount
    );
    activeActorCount = allActorCount;
  }

  void Simulator::updateForce() {
    unsigned int actorCount = actorResources->fluidResource->allocationRange->getAllocatedLength();
    kernels.updateDensityAndPressure(
      actorCount,
      memories.actorStates,
      memories.fluids,
      memories.constants
    );

    kernels.updateFluidForce(
      actorCount,
      memories.actorStates,
      memories.fluids,
      memories.constants
    );
  }

  void Simulator::motion() {
    unsigned int actorCount = actorResources->fluidResource->allocationRange->getAllocatedLength();
    kernels.moveFluid(
      actorCount,
      memories.fluids,
      memories.actorStates,
      memories.physicalQuantities,
      memories.constants
    );
  }
}