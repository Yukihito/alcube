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
#include "opencl/dtos.h"
#include "SoftBodyParticle.h"
#include "Spring.h"
#include "../utils/opencl/Simulator.h"
#include "FluidParticle.h"

namespace alcube::physics {
  using namespace utils::opencl::conversions;
  class Dtos {
    public:
      opencl::dtos::Grid* grid;
      opencl::dtos::Actor* actors;
      opencl::dtos::ActorState* actorStates;
      opencl::dtos::RigidBodyState* currentStates;
      opencl::dtos::RigidBodyState* nextStates;
      opencl::dtos::GridAndActorRelation* gridAndActorRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
      opencl::dtos::Spring* springs;
      opencl::dtos::FluidSettings* fluidSettings;
      opencl::dtos::FluidState* inputFluidStates;
      opencl::dtos::FluidState* fluidStates;
      opencl::dtos::Constants* constants;
  };

  class Memories {
    public:
      utils::opencl::Memory* grid;
      utils::opencl::Memory* actors;
      utils::opencl::Memory* actorStates;
      utils::opencl::Memory* currentStates;
      utils::opencl::Memory* nextStates;
      utils::opencl::Memory* gridAndActorRelations;
      utils::opencl::Memory* gridStartIndices;
      utils::opencl::Memory* gridEndIndices;
      utils::opencl::Memory* springs;
      utils::opencl::Memory* springVars;
      utils::opencl::Memory* fluidSettings;
      utils::opencl::Memory* inputFluidStates;
      utils::opencl::Memory* fluidStates;
      utils::opencl::Memory* constants;
  };

  class Kernels {
    public:
      cl_kernel fillGridIndex;
      cl_kernel merge;
      cl_kernel bitonic;
      cl_kernel setGridRelationIndexRange;
      cl_kernel initGridAndActorRelations;
      cl_kernel collectIntersections;
      cl_kernel updateByPenaltyImpulse;
      cl_kernel updateByFrictionalImpulse;
      cl_kernel collectCollisions;
      cl_kernel updateByConstraintImpulse;
      cl_kernel calcSpringImpulses;
      cl_kernel updateBySpringImpulse;
      cl_kernel postProcessing;
      cl_kernel inputFluid;
      cl_kernel updateDensityAndPressure;
      cl_kernel updateFluidForce;
      cl_kernel moveFluid;
      cl_kernel inputConstants;
  };

  class Simulator : public utils::opencl::Simulator {
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
      Dtos dtos;
      Memories memories;
      Kernels kernels;
    private:
      std::vector<SoftBodyParticle*> softBodyParticles;
      std::vector<FluidParticle*> fluidParticles;
      std::vector<Spring*> springs;
      unsigned int allGridCount;
      unsigned int softBodyParticleCount;
      unsigned int maxActorCount;
      unsigned int actorCount;
      unsigned int actorCountForBitonicSort;
      unsigned int maxActorCountForBitonicSort;
      unsigned int springCount;
      unsigned int maxSpringCount;
      unsigned int fluidParticleCount;
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
