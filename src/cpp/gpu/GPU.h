#ifndef ALCUBE_GPU_H
#define ALCUBE_GPU_H

#include "../utils/FileUtil.h"
#include "../utils/opencl/ProgramFactory.h"
#include "../utils/opencl/MemoryManager.h"
#include "../utils/opencl/KernelFactory.h"
#include "../utils/opencl/CommandQueue.h"
#include "../utils/opencl/ResourcesProvider.h"
#include "../utils/opencl/Memory.h"
#include "dtos.h"

namespace alcube::gpu {
  namespace memories {
    class Actor {
      public:
        utils::opencl::Memory* memory;
        dtos::Actor* dto;
        dtos::Actor* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class ActorState {
      public:
        utils::opencl::Memory* memory;
        dtos::ActorState* dto;
        dtos::ActorState* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Constants {
      public:
        utils::opencl::Memory* memory;
        dtos::Constants* dto;
        dtos::Constants* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class FluidSettings {
      public:
        utils::opencl::Memory* memory;
        dtos::FluidSettings* dto;
        dtos::FluidSettings* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class FluidState {
      public:
        utils::opencl::Memory* memory;
        dtos::FluidState* dto;
        dtos::FluidState* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Grid {
      public:
        utils::opencl::Memory* memory;
        dtos::Grid* dto;
        dtos::Grid* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class GridAndActorRelation {
      public:
        utils::opencl::Memory* memory;
        dtos::GridAndActorRelation* dto;
        dtos::GridAndActorRelation* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Intersection {
      public:
        utils::opencl::Memory* memory;
        dtos::Intersection* dto;
        dtos::Intersection* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class RigidBodyState {
      public:
        utils::opencl::Memory* memory;
        dtos::RigidBodyState* dto;
        dtos::RigidBodyState* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Spring {
      public:
        utils::opencl::Memory* memory;
        dtos::Spring* dto;
        dtos::Spring* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class SpringVar {
      public:
        utils::opencl::Memory* memory;
        dtos::SpringVar* dto;
        dtos::SpringVar* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class CharMemory {
      public:
        utils::opencl::Memory* memory;
        char* dto;
        char* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class DoubleMemory {
      public:
        utils::opencl::Memory* memory;
        double* dto;
        double* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class FloatMemory {
      public:
        utils::opencl::Memory* memory;
        float* dto;
        float* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Float3Memory {
      public:
        utils::opencl::Memory* memory;
        cl_float3* dto;
        cl_float3* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Float4Memory {
      public:
        utils::opencl::Memory* memory;
        cl_float4* dto;
        cl_float4* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class IntMemory {
      public:
        utils::opencl::Memory* memory;
        int* dto;
        int* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class LongMemory {
      public:
        utils::opencl::Memory* memory;
        long* dto;
        long* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class ShortMemory {
      public:
        utils::opencl::Memory* memory;
        short* dto;
        short* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class UcharMemory {
      public:
        utils::opencl::Memory* memory;
        unsigned char* dto;
        unsigned char* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class UintMemory {
      public:
        utils::opencl::Memory* memory;
        unsigned int* dto;
        unsigned int* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class UlongMemory {
      public:
        utils::opencl::Memory* memory;
        unsigned long* dto;
        unsigned long* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class UshortMemory {
      public:
        utils::opencl::Memory* memory;
        unsigned short* dto;
        unsigned short* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };
  }

  class Memories {
    public:
      memories::Grid grid;
      memories::Actor actors;
      memories::RigidBodyState currentStates;
      memories::Spring springs;
      memories::FluidState inputFluidStates;
      memories::FluidSettings fluidSettings;
      memories::ActorState actorStates;
      memories::RigidBodyState nextStates;
      memories::GridAndActorRelation gridAndActorRelations;
      memories::UintMemory gridStartIndices;
      memories::UintMemory gridEndIndices;
      memories::SpringVar springVars;
      memories::FluidState fluidStates;
      memories::Constants constants;
  };

  class Dtos {
    public:
      dtos::Grid* grid;
      dtos::Actor* actors;
      dtos::RigidBodyState* currentStates;
      dtos::Spring* springs;
      dtos::FluidState* inputFluidStates;
      dtos::FluidSettings* fluidSettings;
      dtos::ActorState* actorStates;
      dtos::RigidBodyState* nextStates;
      dtos::GridAndActorRelation* gridAndActorRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
      dtos::SpringVar* springVars;
      dtos::FluidState* fluidStates;
      dtos::Constants* constants;
  };

  class RawKernels {
    public:
      cl_kernel inputConstants;
      cl_kernel initGridAndActorRelations;
      cl_kernel fillGridIndex;
      cl_kernel merge;
      cl_kernel bitonic;
      cl_kernel setGridRelationIndexRange;
      cl_kernel updateByPenaltyImpulse;
      cl_kernel updateByFrictionalImpulse;
      cl_kernel collectCollisions;
      cl_kernel updateByConstraintImpulse;
      cl_kernel inputFluid;
      cl_kernel updateDensityAndPressure;
      cl_kernel updateFluidForce;
      cl_kernel moveFluid;
      cl_kernel postProcessing;
      cl_kernel collectIntersections;
      cl_kernel calcSpringImpulses;
      cl_kernel updateBySpringImpulse;
  };

  class Kernels {
    public:
      utils::opencl::CommandQueue* queue;
      RawKernels rawKernels;
      void inputConstants(unsigned int workSize, memories::Constants& constants, memories::Grid& grid, memories::FluidSettings& fluidSettings, float gravityAcceleration, float deltaTime, float splitDeltaTime, float sphericalShellRadius, unsigned short rigidBodyParticleCount, unsigned short fluidParticleCount);
      void initGridAndActorRelations(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int gridIndex, unsigned short actorIndex);
      void fillGridIndex(unsigned int workSize, memories::Grid& grid, memories::Actor& actors, memories::ActorState& actorStates, memories::RigidBodyState& currentStates, memories::RigidBodyState& nextStates, memories::GridAndActorRelation& relations);
      void merge(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance);
      void bitonic(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance, unsigned int stageDistance);
      void setGridRelationIndexRange(unsigned int workSize, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, unsigned int actorCount);
      void updateByPenaltyImpulse(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates, float deltaTime);
      void updateByFrictionalImpulse(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates);
      void collectCollisions(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates);
      void updateByConstraintImpulse(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates);
      void inputFluid(unsigned int workSize, memories::FluidState& inputFluidStates, memories::FluidState& fluidStates);
      void updateDensityAndPressure(unsigned int workSize, memories::ActorState& actorStates, memories::FluidState& fluidStates, memories::Constants& constants);
      void updateFluidForce(unsigned int workSize, memories::ActorState& actorStates, memories::FluidState& fluidStates, memories::Constants& constants);
      void moveFluid(unsigned int workSize, memories::FluidState& fluidStates, memories::RigidBodyState& nextStates, memories::Constants& constants);
      void postProcessing(unsigned int workSize, memories::Grid& grid, memories::Actor& actors, memories::ActorState& actorStates, memories::RigidBodyState& nextStates, float deltaTime);
      void collectIntersections(unsigned int workSize, memories::ActorState& actorStates, memories::Spring& springs, memories::RigidBodyState& nextStates, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, memories::Constants& constants);
      void calcSpringImpulses(unsigned int workSize, memories::ActorState& actorStates, memories::Spring& springs, memories::SpringVar& springVars, memories::RigidBodyState& nextStates, float deltaTime);
      void updateBySpringImpulse(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates, memories::RigidBodyState& nextStates, memories::SpringVar& springVars, float deltaTime);
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

