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
#include "opencl/dtos.h"
#include "SoftBodyParticle.h"
#include "Spring.h"
#include "../utils/opencl/Simulator.h"
#include "FluidParticle.h"
#include "SimulatorBase.h"
#include "../utils/alcubemath.h"

namespace alcube::physics {
  using namespace utils::opencl::conversions;


  class Simulator : public SimulatorBase {
    public:
      explicit Simulator(
        utils::opencl::Resources* resources,
        utils::FileUtil* fileUtil,
        unsigned int maxActorCount,
        unsigned int gridEdgeLength,
        unsigned int xGridCount,
        unsigned int yGridCount,
        unsigned int zGridCount
      );

      void add(SoftBodyParticle* softBodyParticle);
      void add(Spring* spring);
      void add(FluidParticle* fluidParticle);
      SoftBodyParticle* getSoftBodyParticle(unsigned long i);
      void update(float deltaTime) override;
      float gravity;
      float sphericalShellRadius;
    private:
      std::vector<SoftBodyParticle*> softBodyParticles;
      std::vector<FluidParticle*> fluidParticles;
      std::vector<Spring*> springs;

      bool initialized;
      void setUpComputingSize();
      void setUpMemories();
      void initGPUMemory(float deltaTime);
      void input();
      void output();
      void computeBroadPhase();
      void computeNarrowPhase();
      void resolveConstraints(float deltaTime);
      void motion(float deltaTime);
      void updateFluid(float deltaTime);

      void setUpSpring(unsigned int springIndex, unsigned char nodeIndex);
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
