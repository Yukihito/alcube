#ifndef ALCUBE_PHYSICS_SIMULATOR_H
#define ALCUBE_PHYSICS_SIMULATOR_H

#include <vector>
#include <random>
#include "../utils/opencl/CommandQueue.h"
#include "../utils/opencl/conversions.h"
#include "../utils/opencl/KernelFactory.h"
#include "../utils/opencl/Memory.h"
#include "../utils/opencl/MemoryManager.h"
#include "../utils/opencl/ProgramFactory.h"
#include "../utils/opencl/Resources.h"
#include "../utils/opencl/conversions.h"
#include "../utils/alcubemath.h"
#include "../gpu/GPUAccessor.h"
#include "../utils/opencl/ResourcesProvider.h"
#include "SubSimulator.h"
#include "ActorResources.h"
#include "Grid.h"

namespace alcube::physics {
  using namespace utils::opencl::conversions;

  class Simulator {
    public:
      float gravity;
      float sphericalShellRadius;

      explicit Simulator(
        unsigned int maxActorCount,
        Grid* grid,
        float deltaTime,
        gpu::GPUAccessor* gpuAccessor,
        ActorResources* actorResources
      );
      void add(SubSimulator* subSimulator);
      void setUp();
      void update();
      unsigned short getActorCount();

    private:
      Grid* grid;
      gpu::GPUAccessor* gpuAccessor;
      ActorResources* actorResources;
      gpu::Kernels kernels;
      gpu::Memories memories;
      std::vector<SubSimulator*> subSimulators;

      unsigned int allGridCount;
      unsigned int maxActorCount;
      unsigned int activeActorCount;
      unsigned short allActorCount;
      float deltaTime;

      void setUpConstants();
      void input();
      void updateGPUResourcesCount();
      void computeBroadPhase();
      void computeNarrowPhase();
      void updateForce();
      void motion();
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
