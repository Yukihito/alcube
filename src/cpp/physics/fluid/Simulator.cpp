#include "Simulator.h"

namespace alcube::physics::fluid {
  Simulator::Simulator(gpu::GPUAccessor *gpuAccessor, ActorResources* actorResources) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->actorResources = actorResources;
    actorResources->fluidResource->allocationRange->onAllocationLengthChanged.subscribe([&](){
      unsigned int actorCount = this->actorResources->fluidResource->allocationRange->getAllocatedLength();
      memories.hostFluids.setCount(actorCount);
      memories.fluids.setCount(actorCount);
    });
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

  void Simulator::setUpComputingSize() {}

  void Simulator::writeHostMemories() {}

  void Simulator::setUpMemories() {
    unsigned int actorCount = actorResources->fluidResource->allocationRange->getAllocatedLength();
    memories.hostFluids.write();
    kernels.inputFluids(
      (unsigned short)actorCount,
      memories.hostFluids,
      memories.fluids
    );
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

  bool Simulator::add(physics::Actor *actor) {
    return false;
  }
}