#ifndef ALCUBE_MODEL3D_H
#define ALCUBE_MODEL3D_H

#include "../../gpu/GPUAccessor.h"
#include "../../utils/ResourceAllocation.h"
#include "../../utils/MemoryPool.h"
#include "../../utils/GPUBasedProperty.h"
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

  class RenderingGroupSettings {
    public:
      virtual InstanceColorType getInstanceColorType() = 0;
      virtual bool refersToRotations() = 0;
  };

  class Model3D {
    public:
      void init(
        utils::AllocationRange* actorAllocationRange,
        utils::AllocationRange* allocationRange,
        RenderingGroupSettings* groupSettings,
        gpu::GPUAccessor* gpuAccessor
      );
      glm::vec3 getColor();
      void setColor(glm::vec3 v);

    private:
      utils::AllocationRange* actorAllocationRange;
      utils::AllocationRange* allocationRange;
      models::drawing::RenderingGroupSettings* groupSettings;
      utils::GPUBasedStruct<gpu::dtos::Renderer> featuresStruct = {};
      utils::GPUBasedStruct<cl_float3> colorsStruct = {};
      utils::GPUBasedReference<gpu::dtos::Renderer> actorIndex = {};
      utils::GPUBasedProperty<gpu::dtos::Renderer, int> refersToRotations = {};
      utils::GPUBasedProperty<gpu::dtos::Renderer, unsigned int> instanceColorType = {};
      void setUpAllocations(gpu::GPUAccessor* gpuAccessor);
  };

  class Model3DFactory {
    public:
      explicit Model3DFactory(utils::MemoryPool<Model3D>* memoryPool, gpu::GPUAccessor* gpuAccessor);
      Model3D* create(
        utils::AllocationRange* actorAllocationRange,
        utils::AllocationRange* groupAllocationRange,
        RenderingGroupSettings* groupSettings
      );

    private:
      utils::MemoryPool<Model3D>* memoryPool;
      gpu::GPUAccessor* gpuAccessor;
  };
}


#endif //ALCUBE_MODEL3D_H
