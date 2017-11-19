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

namespace alcube::physics {
  using namespace utils::opencl::conversions;
  class Dtos {
    public:
      opencl::dtos::Grid* grid;
      opencl::dtos::Cell* cells;
      opencl::dtos::RigidBodyState* currentStates;
      opencl::dtos::RigidBodyState* nextStates;
      opencl::dtos::GridAndCellRelation* gridAndCellRelations;
      unsigned int* gridStartIndices;
      unsigned int* gridEndIndices;
  };

  class Memories {
    public:
      utils::opencl::Memory* grid;
      utils::opencl::Memory* cells;
      utils::opencl::Memory* currentStates;
      utils::opencl::Memory* nextStates;
      utils::opencl::Memory* gridAndCellRelations;
      utils::opencl::Memory* gridStartIndices;
      utils::opencl::Memory* gridEndIndices;
  };

  class Kernels {
    public:
      cl_kernel fillGridIndex;
      cl_kernel merge;
      cl_kernel bitonic;
      cl_kernel setGridRelationIndexRange;
      cl_kernel initGridAndCellRelations;
      cl_kernel collectCollisionAndIntersections;
      cl_kernel updatePhysicalQuantities;
      cl_kernel resolveIntersection;
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

      std::vector<Cell*> cells;
      void update(float deltaTime);
      std::mutex* cellsMutex;

      Dtos dtos;
      Memories memories;
      Kernels kernels;
    private:
      unsigned int maxCellCount;
      unsigned int allGridCount;
      utils::opencl::KernelFactory* kernelFactory;
      utils::opencl::ProgramFactory* programFactory;
      utils::opencl::MemoryManager* memoryManager;
      utils::opencl::CommandQueue* queue;
  };
}

#endif //ALCUBE_PHYSICS_SIMULATOR_H
