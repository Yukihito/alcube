#include "RenderingGroup.h"

namespace alcube::models::drawing {
  void RenderingGroup::init(
    alcube::drawing::shaders::Shaders* shaders,
    Settings* settings
  ) {
    this->shaders = shaders;
    this->settings = settings;
    material.ambient = glm::vec3();
    material.diffuse = glm::vec3();
    material.specular = glm::vec3();
    model3Ds = {};
    drawable = nullptr;
    texture = TEXTURE_NONE;
    instanceColorType = INSTANCE_COLOR_TYPE_NONE;
    allocations = {};
  }

  glm::vec3 RenderingGroup::getDiffuse() {
    return material.diffuse;
  }

  void RenderingGroup::setDiffuse(glm::vec3 v) {
    material.diffuse = v;
  }

  glm::vec3 RenderingGroup::getAmbient() {
    return material.ambient;
  }

  void RenderingGroup::setAmbient(glm::vec3 v) {
    material.ambient = v;
  }

  glm::vec3 RenderingGroup::getSpecular() {
    return material.specular;
  }

  void RenderingGroup::setSpecular(glm::vec3 v) {
    material.specular = v;
  }

  Texture RenderingGroup::getTexture() {
    return texture;
  }

  void RenderingGroup::setTexture(alcube::models::drawing::Texture v) {
    texture = v;
  }

  InstanceColorType RenderingGroup::getInstanceColorType() {
    return instanceColorType;
  }

  void RenderingGroup::setInstanceColorType(alcube::models::drawing::InstanceColorType v) {
    instanceColorType = v;
  }

  alcube::drawing::Drawable* RenderingGroup::getDrawable() {
    return drawable;
  }

  void RenderingGroup::allocate(
    alcube::utils::AllocationRange *rendererAllocationRange,
    alcube::gpu::GPUAccessor *gpuAccessor
  ) {
    allocationRange = rendererAllocationRange->allocate((unsigned int)model3Ds.size());
    for (auto model3D : model3Ds) {
      model3D->allocate(this, allocationRange, gpuAccessor);
    }
    allocations.colors.init(allocationRange, gpuAccessor->dtos.colors);
    allocations.positions.init(allocationRange, gpuAccessor->dtos.positions);
    allocations.rotations0.init(allocationRange, gpuAccessor->dtos.rotations0);
    allocations.rotations1.init(allocationRange, gpuAccessor->dtos.rotations1);
    allocations.rotations2.init(allocationRange, gpuAccessor->dtos.rotations2);
    allocations.rotations3.init(allocationRange, gpuAccessor->dtos.rotations3);
  }

  void RenderingGroup::initialize() {
    for (auto model3D : model3Ds) {
      model3D->initialize();
    }
    auto shader = selectShader();
    drawable = new SphereDrawable(
      *shader,
      material,
      settings->world.maxActorCount,
      (GLfloat*)allocations.positions.getPtr(),
      (GLfloat*)allocations.rotations0.getPtr(),
      (GLfloat*)allocations.rotations1.getPtr(),
      (GLfloat*)allocations.rotations2.getPtr(),
      (GLfloat*)allocations.rotations3.getPtr(),
      (GLfloat*)allocations.colors.getPtr()
    );
    if (texture == TEXTURE_CHECK) {
      drawable->texture = new alcube::drawing::textures::CheckTexture(128, 128);
    }
    drawable->shape->instanceCount = allocationRange->getLength();
  }

  void RenderingGroup::add(alcube::models::drawing::Model3D *model3D) {
    model3Ds.push_back(model3D);
  }

  alcube::drawing::Shader* RenderingGroup::selectShader() {
    alcube::drawing::Shader* shader = nullptr;
    if (instanceColorType == INSTANCE_COLOR_TYPE_NONE) {
      if (texture == TEXTURE_CHECK) {
        shader = &shaders->directionalLight.textureSingleColor;
      } else {
        shader = &shaders->directionalLight.singleColor;
      }
    } else {
      if (texture == TEXTURE_CHECK) {
        shader = &shaders->directionalLight.textureInstanceColorShader;
      } else {
        shader = &shaders->directionalLight.instanceColor;
      }
    }
    return shader;
  }

  bool RenderingGroup::refersToRotations() {
    return texture != TEXTURE_NONE;
  }

  RenderingGroupFactory::RenderingGroupFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::RenderingGroup> *memoryPool,
    alcube::drawing::shaders::Shaders *shaders,
    alcube::models::Settings *settings
  ) {
    this->memoryPool = memoryPool;
    this->shaders = shaders;
    this->settings = settings;
  }

  RenderingGroup* RenderingGroupFactory::create() {
    auto renderer = memoryPool->get();
    renderer->init(shaders, settings);
    return renderer;
  }
}
