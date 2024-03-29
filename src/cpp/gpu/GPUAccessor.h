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
        void write(size_t offset);
        void read();
        void read(Actor* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(ActorState* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Constants* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Fluid* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(FluidSettings* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Grid* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(GridAndActorRelation* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Intersection* dst);
        void readRange(size_t min, size_t length);
        void zeroFill();
        void setCount(size_t count);
        utils::opencl::ResourcesProvider* resourcesProvider;
    };

    class Renderer {
      public:
        utils::opencl::Memory* memory;
        dtos::Renderer* dto;
        dtos::Renderer* at(int i);
        void write();
        void write(size_t offset);
        void read();
        void read(Renderer* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(SoftBody* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Spring* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(SpringState* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(CharMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(DoubleMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(FloatMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Float16Memory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Float3Memory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(Float4Memory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(IntMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(LongMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(ShortMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(UcharMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(UintMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(UlongMemory* dst);
        void readRange(size_t min, size_t length);
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
        void write(size_t offset);
        void read();
        void read(UshortMemory* dst);
        void readRange(size_t min, size_t length);
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
      memories::Actor hostActors;
      memories::Actor actors;
      memories::ActorState actorStates;
      memories::SoftBody hostSoftBodies;
      memories::SoftBody softBodies;
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
      memories::Float3Memory hostColors;
      memories::Float3Memory colors;
      memories::Renderer hostRenderers;
      memories::Renderer renderers;
  };

  class Dtos {
    public:
      dtos::Grid* grid;
      dtos::FluidSettings* fluidSettings;
      dtos::Constants* constants;
      dtos::Actor* hostActors;
      dtos::Actor* actors;
      dtos::ActorState* actorStates;
      dtos::SoftBody* hostSoftBodies;
      dtos::SoftBody* softBodies;
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
      cl_float3* hostColors;
      cl_float3* colors;
      dtos::Renderer* hostRenderers;
      dtos::Renderer* renderers;
  };

  class RawKernels {
    public:
      cl_kernel initGridAndActorRelations;
      cl_kernel fillGridIndex;
      cl_kernel merge;
      cl_kernel bitonic;
      cl_kernel setGridRelationIndexRange;
      cl_kernel inputPhysicsConstants;
      cl_kernel inputFluidConstants;
      cl_kernel inputSoftbodyConstants;
      cl_kernel inputActors;
      cl_kernel inputSoftBodies;
      cl_kernel inputSprings;
      cl_kernel inputFluids;
      cl_kernel moveFluid;
      cl_kernel calcSpringImpulses;
      cl_kernel updateBySpringImpulse;
      cl_kernel postProcessing;
      cl_kernel collectIntersections;
      cl_kernel outputActors;
      cl_kernel collectCollisions;
      cl_kernel updateByConstraintImpulse;
      cl_kernel updateDensityAndPressure;
      cl_kernel updateFluidForce;
      cl_kernel updateByFrictionalImpulse;
      cl_kernel initStepVariables;
      cl_kernel updateByPenaltyImpulse;
      cl_kernel inputRenderers;
      cl_kernel updateDrawingBuffer;
  };

  class Kernels {
    public:
      utils::opencl::CommandQueue* queue;
      RawKernels rawKernels;
      void initGridAndActorRelations(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int gridIndex, unsigned short actorIndex);
      void fillGridIndex(unsigned int workSize, memories::Constants& constants, memories::ActorState& actorStates, memories::GridAndActorRelation& relations);
      void merge(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance);
      void bitonic(unsigned int workSize, memories::GridAndActorRelation& relations, unsigned int distance, unsigned int stageDistance);
      void setGridRelationIndexRange(unsigned int workSize, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, unsigned int actorCount);
      void inputPhysicsConstants(unsigned int workSize, memories::Constants& constants, memories::Grid& grid, float gravityAcceleration, float deltaTime, float sphericalShellRadius);
      void inputFluidConstants(unsigned int workSize, memories::Constants& constants, memories::FluidSettings& fluidSettings);
      void inputSoftbodyConstants(unsigned int workSize, memories::Constants& constants, float splitDeltaTime);
      void inputActors(unsigned int workSize, memories::Actor& hostActors, memories::ActorState& actorStates, unsigned short offset);
      void inputSoftBodies(unsigned int workSize, memories::SoftBody& hostSoftBodies, memories::SoftBody& softBodies, unsigned short offset);
      void inputSprings(unsigned int workSize, memories::Spring& springs, memories::SpringState& springStates, unsigned int offset);
      void inputFluids(unsigned int workSize, memories::Fluid& hostFluids, memories::Fluid& fluids, unsigned short offset);
      void moveFluid(unsigned int workSize, memories::Fluid& fluids, memories::ActorState& actorStates, memories::Constants& constants);
      void calcSpringImpulses(unsigned int workSize, memories::Constants& constants, memories::SpringState& springStates, memories::ActorState& actorStates);
      void updateBySpringImpulse(unsigned int workSize, memories::Constants& constants, memories::SoftBody& softBodies, memories::ActorState& actorStates, memories::SpringState& springStates);
      void postProcessing(unsigned int workSize, memories::Constants& constants, memories::ActorState& actorStates);
      void collectIntersections(unsigned int workSize, memories::ActorState& actorStates, memories::GridAndActorRelation& relations, memories::UintMemory& gridStartIndices, memories::UintMemory& gridEndIndices, memories::Constants& constants);
      void outputActors(unsigned int workSize, memories::Actor& actors, memories::ActorState& actorStates);
      void collectCollisions(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodies);
      void updateByConstraintImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodies);
      void updateDensityAndPressure(unsigned int workSize, memories::ActorState& actorStates, memories::Fluid& fluids, memories::Constants& constants);
      void updateFluidForce(unsigned int workSize, memories::ActorState& actorStates, memories::Fluid& fluids, memories::Constants& constants);
      void updateByFrictionalImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodies);
      void initStepVariables(unsigned int workSize, memories::ActorState& actorStates, memories::Constants& constants);
      void updateByPenaltyImpulse(unsigned int workSize, memories::ActorState& actorStates, memories::SoftBody& softBodies, memories::Constants& constants);
      void inputRenderers(unsigned int workSize, memories::Renderer& hostRenderers, memories::Renderer& renderers, memories::Float3Memory& hostColors, memories::Float3Memory& colors, unsigned int offset);
      void updateDrawingBuffer(unsigned int workSize, memories::Float3Memory& positions, memories::Float3Memory& colors, memories::Float4Memory& rotations0, memories::Float4Memory& rotations1, memories::Float4Memory& rotations2, memories::Float4Memory& rotations3, memories::ActorState& actorStates, memories::Renderer& renderers, unsigned int offset);
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
        unsigned int drawingBufferSize,
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

