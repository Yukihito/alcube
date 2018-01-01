#ifndef ALCUBE_PHYSICS_SIMULATOR_H
#define ALCUBE_PHYSICS_SIMULATOR_H

#include <vector>
#include <mutex>
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
#include "Cell.h"
#include "Spring.h"

namespace alcube::physics {
  using namespace utils::opencl::conversions;
  class Dtos {
    public:
      opencl::dtos::Grid* grid;
      opencl::dtos::Cell* cells;
      opencl::dtos::CellVar* cellVars;
      opencl::dtos::RigidBodyState* currentStates;
      opencl::dtos::RigidBodyState* nextStates;
      opencl::dtos::GridAndCellRelation* gridAndCellRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
      opencl::dtos::Spring* springs;
  };

  class Memories {
    public:
      utils::opencl::Memory* grid;
      utils::opencl::Memory* cells;
      utils::opencl::Memory* cellVars;
      utils::opencl::Memory* currentStates;
      utils::opencl::Memory* nextStates;
      utils::opencl::Memory* gridAndCellRelations;
      utils::opencl::Memory* gridStartIndices;
      utils::opencl::Memory* gridEndIndices;
      utils::opencl::Memory* springs;
      utils::opencl::Memory* springVars;
  };

  class Kernels {
    public:
      cl_kernel fillGridIndex;
      cl_kernel merge;
      cl_kernel bitonic;
      cl_kernel setGridRelationIndexRange;
      cl_kernel initGridAndCellRelations;
      cl_kernel collectIntersections;
      cl_kernel updateByPenaltyImpulse;
      cl_kernel updateByFrictionalImpulse;
      cl_kernel collectCollisions;
      cl_kernel updateByConstraintImpulse;
      cl_kernel calcSpringImpulses;
      cl_kernel updateBySpringImpulse;
      cl_kernel postProcessing;
  };

  class Simulator {
    public:
      explicit Simulator(
        utils::opencl::Resources* resources,
        utils::FileUtil* fileUtil,
        std::mutex* cellsMutex,
        unsigned int maxCellCount,
        unsigned int gridEdgeLength,
        unsigned int xGridCount,
        unsigned int yGridCount,
        unsigned int zGridCount
      );

      void add(Cell* cell);
      void add(Spring* spring);
      Cell* getCell(unsigned long i);
      void update(float deltaTime);
      float gravity;
      Dtos dtos;
      Memories memories;
      Kernels kernels;
      bool allocated;
      void setUpComputingSize();
      void setUpMemories();
      void tearDownMemories();
      void input();
      void output();
      void computeBroadPhase();
      void computeNarrowPhase(float deltaTime);
      void resolveConstraints(float deltaTime);
      void motion(float deltaTime);
      void read(utils::opencl::Memory* memory, void* hostPtr);
      void setUpSpring(unsigned int springIndex, unsigned char nodeIndex);
    private:
      std::mutex* cellsMutex;
      std::vector<Cell*> cells;
      std::vector<Spring*> springs;
      unsigned int allGridCount;
      unsigned int cellCount;
      unsigned int maxCellCount;
      unsigned int cellCountForBitonicSort;
      unsigned int maxCellCountForBitonicSort;
      unsigned int springCount;
      unsigned int maxSpringCount;
      utils::opencl::CommandQueue* queue;
      utils::opencl::KernelFactory* kernelFactory;
      utils::opencl::ProgramFactory* programFactory;
      utils::opencl::MemoryManager* memoryManager;
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
