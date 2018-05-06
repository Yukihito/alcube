#ifndef ALCUBE_DRAWING_RENDERING_GROUP_H
#define ALCUBE_DRAWING_RENDERING_GROUP_H

#include <glm/vec3.hpp>
#include <CL/cl_platform.h>
#include "../../drawing/Drawable.h"
#include "../../utils/MemoryPool.h"
#include "../../gpu/GPUAccessor.h"
#include "SphereDrawable.h"
#include "../Settings.h"
#include <CL/cl_platform.h>
#include "../../drawing/textures/CheckTexture.h"
#include "Model3D.h"

namespace alcube::models::drawing {
  struct RenderingGroupAllocations {
    utils::ResourceAllocation<cl_float3> positions;
    utils::ResourceAllocation<cl_float4> rotations0;
    utils::ResourceAllocation<cl_float4> rotations1;
    utils::ResourceAllocation<cl_float4> rotations2;
    utils::ResourceAllocation<cl_float4> rotations3;
    utils::ResourceAllocation<cl_float3> colors;
  };

  class RenderingGroup : public RenderingGroupSettings {
    public:
      void init(
        alcube::drawing::shaders::Shaders* shaders,
        Settings* settings,
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        Model3DFactory* model3DFactory
      );
      glm::vec3 getDiffuse();
      void setDiffuse(glm::vec3 v);
      glm::vec3 getAmbient();
      void setAmbient(glm::vec3 v);
      glm::vec3 getSpecular();
      void setSpecular(glm::vec3 v);
      Texture getTexture();
      void setTexture(Texture v);

      InstanceColorType  getInstanceColorType() override;
      void setInstanceColorType(InstanceColorType v);
      bool refersToRotations() override;

      Model3D* createModel3D(IndexHolder *indexHolder);
      alcube::drawing::Drawable* getDrawable();
      void input();
      void updateGPU();
      void updateHost();
    private:
      alcube::drawing::Drawable* drawable = nullptr;
      Texture texture = TEXTURE_NONE;
      InstanceColorType instanceColorType = INSTANCE_COLOR_TYPE_NONE;
      alcube::drawing::shaders::Shaders* shaders = nullptr;
      Settings* settings = nullptr;
      utils::AllocationRange* allocationRange = nullptr;
      RenderingGroupAllocations allocations = {};
      gpu::GPUAccessor* gpuAccessor = nullptr;
      Model3DFactory* model3DFactory = nullptr;
      unsigned int allModel3DCount = 0;
      unsigned int activeModel3DCount = 0;

      alcube::drawing::Shader* selectShader();
      void setUpAllocations();
      void setUpDrawable();
      void updateDrawable();
  };

  class RenderingGroupFactory {
    public:
      explicit RenderingGroupFactory(
        utils::MemoryPool<RenderingGroup>* memoryPool,
        alcube::drawing::shaders::Shaders* shaders,
        Settings* settings,
        gpu::GPUAccessor* gpuAccessor,
        Model3DFactory* model3DFactory
      );
      RenderingGroup* create(utils::AllocationRange* rendererAllocationRange);

    private:
      utils::MemoryPool<RenderingGroup>* memoryPool;
      alcube::drawing::shaders::Shaders* shaders;
      Settings* settings;
      gpu::GPUAccessor* gpuAccessor;
      Model3DFactory* model3DFactory;
  };
}

#endif //ALCUBE_DRAWING_RENDERING_GROUP_H
