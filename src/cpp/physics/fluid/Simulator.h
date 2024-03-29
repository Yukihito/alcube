#ifndef ALCUBE_FLUIDSIMULATOR_H
#define ALCUBE_FLUIDSIMULATOR_H

#include "../SubSimulator.h"
#include "../ActorResources.h"
#include "Actor.h"

namespace alcube::physics::fluid {
  class Simulator : public SubSimulator {
    public:
      explicit Simulator(gpu::GPUAccessor* gpuAccessor, ActorResources* actorResources);
      void setUpConstants() override;
      void input() override;
      void updateForce() override;
      void motion() override;

    private:
      ActorResources* actorResources;
      gpu::Kernels kernels;
      gpu::Memories memories;
      unsigned int activeActorCount;
      unsigned int allActorCount;
  };
}

#endif //ALCUBE_FLUIDSIMULATOR_H
