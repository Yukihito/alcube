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
      explicit Simulator(gpu::GPUAccessor* gpuAccessor, ActorResources* actorResources, float deltaTime);
      void setUpConstants() override;
      void input() override;
      void updateForce() override;
      void motion() override;

    private:
      ActorResources* actorResources;
      gpu::Kernels kernels;
      gpu::Memories memories;
      unsigned int motionIterationCount = 8;
      unsigned int constraintResolvingIterationCount = 16;
      unsigned int activeActorCount;
      unsigned int allActorCount;
      unsigned int activeSpringCount;
      unsigned int allSpringCount;
      float deltaTime;
      void inputActors();
      void inputSprings();
  };
}

#endif //ALCUBE_SOFTBODY_SIMULATOR_H
