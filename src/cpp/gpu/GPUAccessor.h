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

    class Fluid {
      public:
        utils::opencl::Memory* memory;
        dtos::Fluid* dto;
        dtos::Fluid* at(int i);
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

    class SoftBody {
      public:
        utils::opencl::Memory* memory;
        dtos::SoftBody* dto;
        dtos::SoftBody* at(int i);
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

    class Float16Memory {
      public:
        utils::opencl::Memory* memory;
        cl_float16* dto;
        cl_float16* at(int i);
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
      memories::SoftBody hostSoftBodys;
      memories::SoftBody softBodys;
      memories::Spring springs;
      memories::SpringState springStates;
      memories::Fluid hostFluids;
      memories::Fluid fluids;
      memories::GridAndActorRelation gridAndActorRelations;
      memories::UintMemory gridStartIndices;
      memories::UintMemory gridEndIndices;
      memories::Float3Memory positions;
      memories::Float4Memory rotations0;
      memories::Float4Memory rotations1;
      memories::Float4Memory rotations2;
      memories::Float4Memory rotations3;
      memories::Float3Memory colors;
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
      dtos::SoftBody* hostSoftBodys;
      dtos::SoftBody* softBodys;
      dtos::Spring* springs;
      dtos::SpringState* springStates;
      dtos::Fluid* hostFluids;
      dtos::Fluid* fluids;
      dtos::GridAndActorRelation* gridAndActorRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
      cl_float3* positions;
      cl_float4* rotations0;
      cl_float4* rotations1;
      cl_float4* rotations2;
      cl_float4* rotations3;
      cl_float3* colors;
  };

  class RawKernels {
    public:
      cl_kernel initGridAndActorRelations;
      cl_kernel fillGridIndex;
      cl_kernel merge;
      cl_kernel bitonic;
      cl_kernel setGridRelationIndexRange;
      cl_kernel inputConstants;
      cl_kernel inputActors;
      cl_kernel inputSoftBodys;
      cl_kernel inputSprings;
      cl_kernel inputFluids;
      cl_kernel moveFluid;
      cl_kernel calcSpringImpulses;
      cl_kernel updateBySpringImpulse;
      cl_kernel postProcessing;
      cl_kernel collectIntersections;
      cl_kernel collectCollisions;
      cl_kernel updateByConstraintImpulse;
      cl_kernel updateDensityAndPressure;
      cl_kernel updateFluidForce;
      cl_kernel updateByFrictionalImpulse;
      cl_kernel initStepVariables;
      cl_kernel updateByPenaltyImpulse;
      cl_kernel updateDrawingBuffer_SingleColor;
      cl_kernel updateDrawingBuffer_InstanceColor;
      cl_kernel updateDrawingBuffer_Texture_SingleColor;
  };

  class Kernels {
    public:
      utils::opencl::CommandQueue* queue;
      RawKernels rawKernels;
      void initGridAndActorRelations(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int gridIndex, unsigned short actorIndex);
      void fillGridIndex(unsigned int workSize, memories::Constants& constants, memories::PhysicalQuantity& physicalQuantities, memories::GridAndActorRelation& relations);
      void merge(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance);
      void bitonic(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance, unsigned int stageDistance);
      void setGridRelationIndexRange(unsigned int workSize, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, unsigned int actorCount);
      void inputConstants(unsigned int workSize, memories::Constants& constants, memories::Grid& grid, memories::FluidSettings& fluidSettings, float gravityAcceleration, float deltaTime, float splitDeltaTime, float sphericalShellRadius);
      void inputActors(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates, memories::PhysicalQuantity& hostPhysicalQuantities, memories::PhysicalQuantity& physicalQuantities);
      void inputSoftBodys(unsigned int workSize, memories::SoftBody& hostSoftBodys, memories::SoftBody& softBodys);
      void inputSprings(unsigned int workSize, memories::Spring& springs, memories::SpringState& springStates);
      void inputFluids(unsigned int workSize, memories::Fluid& hostFluids, memories::Fluid& fluids);
      void moveFluid(unsigned int workSize, memories::Fluid& fluids, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, memories::Constants& constants);
      void calcSpringImpulses(unsigned int workSize, memories::Constants& constants, memories::SpringState& springStates, memories::PhysicalQuantity& physicalQuantities);
      void updateBySpringImpulse(unsigned int workSize, memories::Constants& constants, memories::SoftBody& softBodys, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, memories::SpringState& springStates);
      void postProcessing(unsigned int workSize, memories::Constants& constants, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities);
      void collectIntersections(unsigned int workSize, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, memories::Constants& constants);
      void collectCollisions(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodys);
      void updateByConstraintImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodys);
      void updateDensityAndPressure(unsigned int workSize, memories::ActorState& actorStates, memories::Fluid& fluids, memories::Constants& constants);
      void updateFluidForce(unsigned int workSize, memories::ActorState& actorStates, memories::Fluid& fluids, memories::Constants& constants);
      void updateByFrictionalImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodys);
      void initStepVariables(unsigned int workSize, memories::ActorState& actorStates, memories::PhysicalQuantity& physicalQuantities, memories::Constants& constants);
      void updateByPenaltyImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodys, memories::Constants& constants);
      void updateDrawingBuffer_SingleColor(unsigned int workSize, memories::Float3Memory& positions, memories::PhysicalQuantity& physicalQuantities);
      void updateDrawingBuffer_InstanceColor(unsigned int workSize, memories::Float3Memory& positions, memories::Float3Memory& colors, memories::PhysicalQuantity& physicalQuantities);
      void updateDrawingBuffer_Texture_SingleColor(unsigned int workSize, memories::Float3Memory& positions, memories::Float4Memory& rotations0, memories::Float4Memory& rotations1, memories::Float4Memory& rotations2, memories::Float4Memory& rotations3, memories::PhysicalQuantity& physicalQuantities);
  };

  class GPUAccessor {
    public:
      Kernels kernels;
      Memories memories;
      Dtos dtos;
      utils::opencl::ResourcesProvider* resourcesProvider;
      explicit GPUAccessor(
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

