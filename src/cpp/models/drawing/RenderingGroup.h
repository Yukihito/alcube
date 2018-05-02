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
    utils::ResourceAllocation<cl_float3>* positions;
    utils::ResourceAllocation<cl_float4>* rotations0;
    utils::ResourceAllocation<cl_float4>* rotations1;
    utils::ResourceAllocation<cl_float4>* rotations2;
    utils::ResourceAllocation<cl_float4>* rotations3;
    utils::ResourceAllocation<cl_float3>* colors;
  };

  class RenderingGroup : public RenderingGroupSettings {
    public:
      void init(
        alcube::drawing::shaders::Shaders* shaders,
        Settings* settings
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

      alcube::drawing::Drawable* getDrawable();
      void allocate(utils::AllocationRange* rendererAllocationRange, gpu::GPUAccessor* gpuAccessor);
      void update();
      void add(Model3D* model3D);

    private:
      alcube::drawing::Material material = {};
      alcube::drawing::Drawable* drawable = nullptr;
      Texture texture = TEXTURE_NONE;
      InstanceColorType instanceColorType = INSTANCE_COLOR_TYPE_NONE;
      alcube::drawing::shaders::Shaders* shaders = nullptr;
      Settings* settings = nullptr;
      std::vector<Model3D*> model3Ds = {};
      utils::AllocationRange* allocationRange;
      RenderingGroupAllocations allocations;

      alcube::drawing::Shader* selectShader();
  };

  class RenderingGroupFactory {
    public:
      explicit RenderingGroupFactory(
        utils::MemoryPool<RenderingGroup>* memoryPool,
        alcube::drawing::shaders::Shaders* shaders,
        Settings* settings
      );
      RenderingGroup* create();

    private:
      utils::MemoryPool<RenderingGroup>* memoryPool;
      alcube::drawing::shaders::Shaders* shaders;
      Settings* settings;
  };
}

#endif //ALCUBE_DRAWING_RENDERING_GROUP_H
