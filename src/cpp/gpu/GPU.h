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

    class PhysicalQuantity {
      public:
        utils::opencl::Memory* memory;
        dtos::PhysicalQuantity* dto;
        dtos::PhysicalQuantity* at(int i);
        void write();
        void read();
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class SoftBodyState {
      public:
        utils::opencl::Memory* memory;
        dtos::SoftBodyState* dto;
        dtos::SoftBodyState* at(int i);
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

    class SpringState {
      public:
        utils::opencl::Memory* memory;
        dtos::SpringState* dto;
        dtos::SpringState* at(int i);
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
      memories::FluidSettings fluidSettings;
      memories::Constants constants;
      memories::Actor actors;
      memories::ActorState actorStates;
      memories::PhysicalQuantity hostPhysicalQuantities;
      memories::PhysicalQuantity physicalQuantities;
      memories::SoftBodyState hostSoftBodyStates;
      memories::SoftBodyState softBodyStates;
      memories::Spring springs;
      memories::SpringState springStates;
      memories::FluidState hostFluidStates;
      memories::FluidState fluidStates;
      memories::GridAndActorRelation gridAndActorRelations;
      memories::UintMemory gridStartIndices;
      memories::UintMemory gridEndIndices;
      memories::Float3Memory hostSphereModelVertices;
      memories::Float3Memory sphereModelVertices;
      memories::Float3Memory vertices;
  };

  class Dtos {
    public:
      dtos::Grid* grid;
      dtos::FluidSettings* fluidSettings;
      dtos::Constants* constants;
      dtos::Actor* actors;
      dtos::ActorState* actorStates;
      dtos::PhysicalQuantity* hostPhysicalQuantities;
      dtos::PhysicalQuantity* physicalQuantities;
      dtos::SoftBodyState* hostSoftBodyStates;
      dtos::SoftBodyState* softBodyStates;
      dtos::Spring* springs;
      dtos::SpringState* springStates;
      dtos::FluidState* hostFluidStates;
      dtos::FluidState* fluidStates;
      dtos::GridAndActorRelation* gridAndActorRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
      cl_float3* hostSphereModelVertices;
      cl_float3* sphereModelVertices;
      cl_float3* vertices;
  };

  class RawKernels {
    public:
      cl_kernel inputConstants;
      cl_kernel inputActors;
      cl_kernel inputSoftBodyStates;
      cl_kernel inputSprings;
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
      cl_kernel inputModelVertices;
      cl_kernel transformModel;
  };

  class Kernels {
    public:
      utils::opencl::CommandQueue* queue;
      RawKernels rawKernels;
      void inputConstants(unsigned int workSize, memories::Constants& constants, memories::Grid& grid, memories::FluidSettings& fluidSettings, float gravityAcceleration, float deltaTime, float splitDeltaTime, float sphericalShellRadius);
      void inputActors(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates, memories::PhysicalQuantity& hostPhysicalQuantities, memories::PhysicalQuantity& physicalQuantities);
      void inputSoftBodyStates(unsigned int workSize, memories::SoftBodyState& hostSoftBodyStates, memories::SoftBodyState& softBodyStates);
      void inputSprings(unsigned int workSize, memories::Spring& springs, memories::SpringState& springStates);
      void initGridAndActorRelations(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int gridIndex, unsigned short actorIndex);
      void fillGridIndex(unsigned int workSize, memories::Constants& constants, memories::PhysicalQuantity& physicalQuantities, memories::GridAndActorRelation& relations);
      void merge(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance);
      void bitonic(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance, unsigned int stageDistance);
      void setGridRelationIndexRange(unsigned int workSize, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, unsigned int actorCount);
      void updateByPenaltyImpulse(unsigned int workSize, memories::ActorState& actorStates, float deltaTime);
      void updateByFrictionalImpulse(unsigned int workSize, memories::ActorState& actorStates);
      void collectCollisions(unsigned int workSize, memories::ActorState& actorStates);
      void updateByConstraintImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBodyState& softBodyStates);
      void inputFluid(unsigned int workSize, memories::FluidState& hostFluidStates, memories::FluidState& fluidStates);
      void updateDensityAndPressure(unsigned int workSize, memories::ActorState& actorStates, memories::FluidState& fluidStates, memories::Constants& constants);
      void updateFluidForce(unsigned int workSize, memories::ActorState& actorStates, memories::FluidState& fluidStates, memories::Constants& constants);
      void moveFluid(unsigned int workSize, memories::FluidState& fluidStates, memories::PhysicalQuantity& physicalQuantities, memories::Constants& constants);
      void postProcessing(unsigned int workSize, memories::Constants& constants, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, float deltaTime);
      void collectIntersections(unsigned int workSize, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, memories::Constants& constants);
      void calcSpringImpulses(unsigned int workSize, memories::SpringState& springStates, memories::PhysicalQuantity& physicalQuantities, float deltaTime);
      void updateBySpringImpulse(unsigned int workSize, memories::SoftBodyState& softBodyStates, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, memories::SpringState& springStates, float deltaTime);
      void inputModelVertices(unsigned int workSize, memories::Float3Memory& hostVertices, memories::Float3Memory& vertices);
      void transformModel(unsigned int workSize, memories::Float3Memory& vertices, memories::Float3Memory& modelVertices, unsigned int modelVertexCount, memories::PhysicalQuantity& physicalQuantities);
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
        unsigned int allGridCount,
        unsigned int sphereModelVertexCount,
        unsigned int maxVertexCount
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

