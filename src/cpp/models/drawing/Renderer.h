#ifndef ALCUBE_DRAWING_FEATURES_H
#define ALCUBE_DRAWING_FEATURES_H

#include <glm/vec3.hpp>
#include <CL/cl_platform.h>
#include "../../drawing/Drawable.h"
#include "../../utils/MemoryPool.h"
#include "../../gpu/GPUAccessor.h"
#include "SphereDrawable.h"
#include "../Settings.h"
#include "../../drawing/Drawer.h"
#include <CL/cl_platform.h>
#include "../../drawing/textures/CheckTexture.h"

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

  class Renderer {
    public:
      void init(
        gpu::GPUAccessor* gpuAccessor,
        alcube::drawing::shaders::Shaders* shaders,
        alcube::drawing::Drawer* drawer,
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
      InstanceColorType  getInstanceColorType();
      void setInstanceColorType(InstanceColorType v);
      void setUpResources();
      bool refersToRotations();
      void incrementChildCount();

    private:
      alcube::drawing::Material material = {};
      alcube::drawing::Drawable* drawable = nullptr;
      Texture texture = TEXTURE_NONE;
      InstanceColorType instanceColorType = INSTANCE_COLOR_TYPE_NONE;
      gpu::GPUAccessor* gpuAccessor = nullptr;
      alcube::drawing::shaders::Shaders* shaders = nullptr;
      Settings* settings = nullptr;
      alcube::drawing::Drawer* drawer = nullptr;
      unsigned int childCount = 0;
  };

  class RendererFactory {
    public:
      explicit RendererFactory(
        utils::MemoryPool<Renderer>* memoryPool,
        gpu::GPUAccessor* gpuAccessor,
        alcube::drawing::shaders::Shaders* shaders,
        alcube::drawing::Drawer* drawer,
        Settings* settings
      );
      Renderer* create();

    private:
      utils::MemoryPool<Renderer>* memoryPool;
      gpu::GPUAccessor* gpuAccessor;
      alcube::drawing::shaders::Shaders* shaders;
      alcube::drawing::Drawer* drawer;
      Settings* settings;
  };

  class InstanceRenderer {
    public:
      void init(IndexHolder* indexHolder, gpu::GPUAccessor* gpuAccessor, Renderer* parent);
      Renderer* getParent();
      glm::vec3 getColor();
      void setColor(glm::vec3 v);

    private:
      IndexHolder* indexHolder;
      gpu::GPUAccessor* gpuAccessor;
      Renderer* parent;
  };

  class InstanceRendererFactory {
    public:
      explicit InstanceRendererFactory(utils::MemoryPool<InstanceRenderer>* memoryPool, gpu::GPUAccessor* gpuAccessor);
      InstanceRenderer* create(IndexHolder* indexHolder, Renderer* parent);

    private:
      utils::MemoryPool<InstanceRenderer>* memoryPool;
      gpu::GPUAccessor* gpuAccessor;

  };
}

#endif //ALCUBE_DRAWING_FEATURES_H
