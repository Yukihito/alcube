#ifndef ALCUBE_PHYSICS_SIMULATORBASE_H
#define ALCUBE_PHYSICS_SIMULATORBASE_H
#include "../utils/opencl/Simulator.h"
#include "opencl/dtos.h"

namespace alcube::physics {
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
  class SimulatorBase : public utils::opencl::Simulator {
    public:
      explicit SimulatorBase(
        utils::opencl::Resources* resources,
        utils::FileUtil* fileUtil,
        unsigned int maxActorCount,
        unsigned int maxActorCountForBitonicSort,
        unsigned int maxSpringCount,
        unsigned int allGridCount
      );
      Dtos dtos;
      Memories memories;
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
  };
}

#endif //ALCUBE_PHYSICS_SIMULATORBASE_H
