#ifndef ALCUBE_MODEL3D_H
#define ALCUBE_MODEL3D_H

#include "../../gpu/GPUAccessor.h"
#include "../../utils/ResourceAllocation.h"
#include "../../utils/MemoryPool.h"
#include <glm/vec3.hpp>
#include <random>

namespace alcube::models::drawing {
  enum Texture {
    TEXTURE_NONE = 0,
    TEXTURE_CHECK
  };

  enum InstanceColorType {
    INSTANCE_COLOR_TYPE_NONE = 0,
    INSTANCE_COLOR_TYPE_MANUAL,
    INSTANCE_COLOR_TYPE_RANDOM,
    INSTANCE_COLOR_TYPE_LINEAR_MOMENTUM
  };

  class IndexHolder {
    public:
      virtual unsigned int getIndex() = 0;
  };

  class RenderingGroupSettings {
    public:
      virtual InstanceColorType getInstanceColorType() = 0;
      virtual bool refersToRotations() = 0;
  };

  struct Model3DAllocations {
    utils::ResourceAllocation<cl_float3> colors;
    utils::ResourceAllocation<gpu::dtos::Renderer> features;
  };

  class Model3D {
    public:
      void init(
        IndexHolder* actorIndexHolder,
        utils::AllocationRange* allocationRange,
        RenderingGroupSettings* groupSettings,
        gpu::GPUAccessor* gpuAccessor
      );
      glm::vec3 getColor();
      void setColor(glm::vec3 v);

    private:
      IndexHolder* actorIndexHolder;
      utils::AllocationRange* allocationRange;
      Model3DAllocations allocations;
      models::drawing::RenderingGroupSettings* groupSettings;
      void setUpAllocations(gpu::GPUAccessor* gpuAccessor);
  };

  class Model3DFactory {
    public:
      explicit Model3DFactory(utils::MemoryPool<Model3D>* memoryPool, gpu::GPUAccessor* gpuAccessor);
      Model3D* create(IndexHolder* actorIndexHolder, utils::AllocationRange* groupAllocationRange, RenderingGroupSettings* groupSettings);

    private:
      utils::MemoryPool<Model3D>* memoryPool;
      gpu::GPUAccessor* gpuAccessor;
  };
}


#endif //ALCUBE_MODEL3D_H
