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
      void setUpMemories() override;
      void updateForce() override;
      void motion() override;

    private:
      ActorResources* actorResources;
      gpu::Kernels kernels;
      gpu::Memories memories;
  };
}

#endif //ALCUBE_FLUIDSIMULATOR_H
