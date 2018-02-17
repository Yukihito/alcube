#ifndef ALCUBE_GPU_H
#define ALCUBE_GPU_H

#include "../utils/opencl/Simulator.h"
#include "dtos.h"

namespace alcube::gpu {
  namespace memories {
    class Grid {
      public:
        utils::opencl::Memory* memory;
        dtos::Grid* dto;
        dtos::Grid& at(int i);
    };

    class FluidSettings {
      public:
        utils::opencl::Memory* memory;
        dtos::FluidSettings* dto;
        dtos::FluidSettings& at(int i);
    };

    class FluidState {
      public:
        utils::opencl::Memory* memory;
        dtos::FluidState* dto;
        dtos::FluidState& at(int i);
    };
  }

  class Memories {
    public:
      memories::Grid grid;
      memories::FluidSettings fluidSettings;
      memories::FluidState inputFluidStates;
      memories::FluidState fluidStates;
  };

  class Dtos {
    public:
      dtos::Grid* grid;
      dtos::Actor* actors;
      dtos::ActorState* actorStates;
      dtos::RigidBodyState* currentStates;
      dtos::RigidBodyState* nextStates;
      dtos::GridAndActorRelation* gridAndActorRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
      dtos::Spring* springs;
      dtos::FluidSettings* fluidSettings;
      dtos::FluidState* inputFluidStates;
      dtos::FluidState* fluidStates;
      dtos::Constants* constants;
  };

  class RawKernels {
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

  class Kernels {
    public:
      utils::opencl::CommandQueue* queue;
      void inputFluid(unsigned int workSize, memories::FluidState& inputFluidStates, memories::FluidState &fluidState);
      RawKernels rawKernels;
  };

  class GPU {
    public:
      Kernels kernels;
      Memories memories;
      Dtos dtos;
      utils::opencl::ResourcesProvider* resourcesProvider;
      explicit GPU(
        utils::opencl::ResourcesProvider* resourcesProvider,
        unsigned int maxActorCount,
        unsigned int maxActorCountForBitonicSort,
        unsigned int maxSpringCount,
        unsigned int allGridCount
      );
    private:
      utils::opencl::Memory* defineHostMemory(
        const std::string& name,
        size_t size,
        void* hostPtr,
        size_t allocationCount
      );

      utils::opencl::Memory* defineGPUMemory(
        const std::string& name,
        size_t size,
        size_t allocationCount
      );
  };

}

#endif //ALCUBE_GPU_H
