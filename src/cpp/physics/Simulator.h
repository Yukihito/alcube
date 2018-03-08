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
#include "SoftBodyParticle.h"
#include "Spring.h"
#include "FluidParticle.h"
#include "../utils/alcubemath.h"
#include "../gpu/GPU.h"
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
        gpu::GPU* gpu
      );
      void add(SoftBodyParticle* softBodyParticle);
      void add(Spring* spring);
      void add(FluidParticle* fluidParticle);
      SoftBodyParticle* getSoftBodyParticle(unsigned long i);
      void input();
      void update();
      void output();
      float gravity;
      float sphericalShellRadius;
      unsigned int actorCount;
    private:
      gpu::Kernels kernels;
      gpu::Memories memories;
      std::vector<SoftBodyParticle*> softBodyParticles;
      std::vector<FluidParticle*> fluidParticles;
      std::vector<Spring*> springs;

      unsigned int allGridCount;
      unsigned int softBodyParticleCount;
      unsigned int maxActorCount;
      unsigned int actorCountForBitonicSort;
      unsigned int springCount;
      unsigned int fluidParticleCount;
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
      void updateFluid();

      void setUpSpring(unsigned int springIndex, unsigned char nodeIndex);
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
