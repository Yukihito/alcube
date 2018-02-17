#ifndef ALCUBE_PHYSICS_SIMULATORBASE_H
#define ALCUBE_PHYSICS_SIMULATORBASE_H
#include "../utils/opencl/Simulator.h"
#include "opencl/dtos.h"

namespace alcube::physics {
  namespace memories {
    class Grid {
      public:
        utils::opencl::Memory* memory;
        opencl::dtos::Grid* dto;
        opencl::dtos::Grid& at(int i);
    };

    class FluidSettings {
      public:
        utils::opencl::Memory* memory;
        opencl::dtos::FluidSettings* dto;
        opencl::dtos::FluidSettings& at(int i);
    };

    class FluidState {
      public:
        utils::opencl::Memory* memory;
        opencl::dtos::FluidState* dto;
        opencl::dtos::FluidState& at(int i);
    };
  }

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
      memories::Grid grid;
      memories::FluidSettings fluidSettings;
      memories::FluidState inputFluidStates;
      memories::FluidState fluidStates;

      utils::opencl::Memory* _grid;
      utils::opencl::Memory* _actors;
      utils::opencl::Memory* _actorStates;
      utils::opencl::Memory* _currentStates;
      utils::opencl::Memory* _nextStates;
      utils::opencl::Memory* _gridAndActorRelations;
      utils::opencl::Memory* _gridStartIndices;
      utils::opencl::Memory* _gridEndIndices;
      utils::opencl::Memory* _springs;
      utils::opencl::Memory* _springVars;
      utils::opencl::Memory* _fluidSettings;
      utils::opencl::Memory* _inputFluidStates;
      utils::opencl::Memory* _fluidStates;
      utils::opencl::Memory* _constants;
  };

  class _Kernels {
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
      _Kernels rawKernels;
      void inputFluid(unsigned int workSize, memories::FluidState& inputFluidStates, memories::FluidState &fluidState);
  };

  class SimulatorBase : public utils::opencl::Simulator {
    public:
      explicit SimulatorBase(
        utils::opencl::ResourcesProvider* resourcesProvider,
        unsigned int maxActorCount,
        unsigned int maxActorCountForBitonicSort,
        unsigned int maxSpringCount,
        unsigned int allGridCount
      );
      Dtos dtos;
      Memories memories;
      _Kernels _kernels;
      Kernels kernels;
    protected:
      unsigned int allGridCount;
      unsigned int softBodyParticleCount;
      unsigned int maxActorCount;
      unsigned int actorCount;
      unsigned int actorCountForBitonicSort;
      unsigned int maxActorCountForBitonicSort;
      unsigned int springCount;
      unsigned int maxSpringCount;
      unsigned int fluidParticleCount;

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

#endif //ALCUBE_PHYSICS_SIMULATORBASE_H
