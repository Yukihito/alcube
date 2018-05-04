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

namespace alcube::physics {
  using namespace utils::opencl::conversions;

  class Simulator {
    public:
      explicit Simulator(
        unsigned int maxActorCount,
        unsigned int gridEdgeLength,
        unsigned int xGridCount,
        unsigned int yGridCount,
        unsigned int zGridCount,
        float deltaTime,
        gpu::GPUAccessor* gpuAccessor
      );
      void add(Actor* actor);
      void add(SubSimulator* subSimulator);
      void setUp();
      void update();
      float gravity;
      float sphericalShellRadius;
      unsigned int actorCount;
    private:
      unsigned int gridEdgeLength;
      unsigned int xGridCount;
      unsigned int yGridCount;
      unsigned int zGridCount;
      gpu::GPUAccessor* gpuAccessor;
      gpu::Kernels kernels;
      gpu::Memories memories;
      std::vector<Actor*> actors;
      std::vector<SubSimulator*> subSimulators;

      unsigned int allGridCount;
      unsigned int maxActorCount;
      unsigned int actorCountForBitonicSort;
      float deltaTime;

      void setUpConstants();
      void setUpComputingSize();
      void setUpMemories();
      void writeHostMemories();
      void computeBroadPhase();
      void computeNarrowPhase();
      void updateForce();
      void motion();
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
