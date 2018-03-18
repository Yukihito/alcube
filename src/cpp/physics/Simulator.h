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
#include "SoftBodyActor.h"
#include "Spring.h"
#include "FluidActor.h"
#include "../utils/alcubemath.h"
#include "../gpu/GPUAccessor.h"
#include "../utils/opencl/ResourcesProvider.h"

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
        gpu::GPUAccessor* gpu
      );
      void add(SoftBodyActor* actor);
      void add(Spring* spring);
      void add(FluidActor* actor);
      SoftBodyActor* getSoftBodyActor(unsigned long i);
      void input();
      void update();
      void output();
      float gravity;
      float sphericalShellRadius;
      unsigned int actorCount;
    private:
      gpu::Kernels kernels;
      gpu::Memories memories;
      std::vector<SoftBodyActor*> softBodyActors;
      std::vector<FluidActor*> fluidActors;
      std::vector<Spring*> springs;

      unsigned int allGridCount;
      unsigned int softBodyActorCount;
      unsigned int maxActorCount;
      unsigned int actorCountForBitonicSort;
      unsigned int springCount;
      unsigned int fluidActorCount;
      unsigned int motionIterationCount;
      unsigned int constraintResolvingIterationCount;
      float deltaTime;

      void setUpComputingSize();
      void setUpMemories();
      void writeHostMemories();
      void computeBroadPhase();
      void computeNarrowPhase();
      void resolveConstraints();
      void motion();

      void setUpSpring(unsigned int springIndex, unsigned char nodeIndex);
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
