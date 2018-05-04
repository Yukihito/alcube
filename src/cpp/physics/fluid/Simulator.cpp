#include "Simulator.h"

namespace alcube::physics::fluid {
  Simulator::Simulator(gpu::GPUAccessor *gpuAccessor, ActorResources* actorResources) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->actorResources = actorResources;
  }

  void Simulator::setUpConstants() {
    auto fluidSettings = memories.fluidSettings.at(0);
    fluidSettings->stiffness = Actor::stiffness; //64.0f;
    fluidSettings->density = Actor::density; //0.02f;
    fluidSettings->viscosity = Actor::viscosity; //8.0f;
    fluidSettings->particleMass = Actor::mass; //(4.0f / 3.0f) * CL_M_PI_F * CL_M_PI_F * CL_M_PI_F * fluidSettings->density;
    fluidSettings->effectiveRadius = 2.0f;
    fluidSettings->poly6Constant = 315.0f / (64.0f * CL_M_PI_F * powf(fluidSettings->effectiveRadius, 9));
    fluidSettings->spikyGradientConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
    fluidSettings->viscosityLaplacianConstant = 45.0f / (CL_M_PI_F * powf(fluidSettings->effectiveRadius, 6));
  }

  void Simulator::setUpComputingSize() {
    actorCount = (unsigned int)actors.size();
  }

  void Simulator::writeHostMemories() {
    for (unsigned int i = 0; i < actorCount; i++) {
      auto actor = actors[i];
      actor->getPhysicalQuantityDto()->radius = memories.fluidSettings.at(0)->effectiveRadius / 2.0f;
      actor->getPhysicalQuantityDto()->mass = memories.fluidSettings.at(0)->particleMass;
    }
  }

  void Simulator::setUpMemories() {
    memories.hostFluids.setCount(actorCount);
    memories.fluids.setCount(actorCount);
    memories.hostFluids.write();
    kernels.inputFluids(
      (unsigned short)actorCount,
      memories.hostFluids,
      memories.fluids
    );
  }

  void Simulator::updateForce() {
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
    kernels.moveFluid(
      actorCount,
      memories.fluids,
      memories.actorStates,
      memories.physicalQuantities,
      memories.constants
    );
  }

  bool Simulator::add(physics::Actor *actor) {
    if (Actor::instances.count(actor) > 0) {
      actors.push_back(Actor::instances[actor]);
      return true;
    } else {
      return false;
    }
  }
}