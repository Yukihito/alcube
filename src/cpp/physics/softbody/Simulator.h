#ifndef ALCUBE_SOFTBODY_SIMULATOR_H
#define ALCUBE_SOFTBODY_SIMULATOR_H

#include "../SubSimulator.h"
#include "Actor.h"
#include "Spring.h"
#include "../../utils/opencl/conversions.h"
#include "../ActorResources.h"

namespace alcube::physics::softbody {
  class Simulator : public SubSimulator {
    public:
      explicit Simulator(gpu::GPUAccessor* gpuAccessor, ActorResources* actorResources);
      void setUpConstants() override;
      void setUpComputingSize() override;
      void writeHostMemories() override;
      void setUpMemories() override;
      void updateForce() override;
      void motion() override;
      bool add(physics::Actor* actor) override;
      void add(Spring* spring);

    private:
      void setUpSpring(unsigned int springIndex, unsigned char nodeIndex);
      ActorResources* actorResources;
      gpu::Kernels kernels = {};
      gpu::Memories memories = {};
      unsigned int actorCount = 0;
      unsigned int springCount = 0;
      unsigned int motionIterationCount = 8;
      unsigned int constraintResolvingIterationCount = 16;
      std::vector<softbody::Actor*> actors = {};
      std::vector<softbody::Spring*> springs = {};
  };
}

#endif //ALCUBE_SOFTBODY_SIMULATOR_H
