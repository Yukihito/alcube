#include "RenderingGroup.h"

namespace alcube::models::drawing {
  void RenderingGroup::init(
    alcube::drawing::shaders::Shaders* shaders,
    Settings* settings,
    gpu::GPUAccessor* gpuAccessor,
    utils::AllocationRange* allocationRange,
    Model3DFactory* model3DFactory
  ) {
    this->shaders = shaders;
    this->settings = settings;
    this->gpuAccessor = gpuAccessor;
    this->allocationRange = allocationRange;
    this->model3DFactory = model3DFactory;
    texture = TEXTURE_NONE;
    instanceColorType = INSTANCE_COLOR_TYPE_NONE;
    allModel3DCount = 0;
    activeModel3DCount = 0;
    setUpAllocations();
    setUpDrawable();
  }

  glm::vec3 RenderingGroup::getDiffuse() {
    return drawable->material.diffuse;
  }

  void RenderingGroup::setDiffuse(glm::vec3 v) {
    drawable->material.diffuse = v;
  }

  glm::vec3 RenderingGroup::getAmbient() {
    return drawable->material.ambient;
  }

  void RenderingGroup::setAmbient(glm::vec3 v) {
    drawable->material.ambient = v;
  }

  glm::vec3 RenderingGroup::getSpecular() {
    return drawable->material.specular;
  }

  void RenderingGroup::setSpecular(glm::vec3 v) {
    drawable->material.specular = v;
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

  Model3D* RenderingGroup::createModel3D(utils::AllocationRange* actorAllocationRange) {
    auto entity = model3DFactory->create(actorAllocationRange, allocationRange, this);
    return entity;
  }

  void RenderingGroup::input() {
    allModel3DCount = allocationRange->getAllocatedLength();
    auto updateCount = allModel3DCount - activeModel3DCount;
    if (updateCount == 0) {
      return;
    }

    gpuAccessor->kernels.inputRenderers(
      updateCount,
      gpuAccessor->memories.hostRenderers,
      gpuAccessor->memories.renderers,
      gpuAccessor->memories.hostColors,
      gpuAccessor->memories.colors,
      allocationRange->getIndex() + activeModel3DCount
    );
    activeModel3DCount = allModel3DCount;
  }

  void RenderingGroup::setUpAllocations() {
    allocations.colors.init(allocationRange, gpuAccessor->dtos.colors);
    allocations.positions.init(allocationRange, gpuAccessor->dtos.positions);
    allocations.rotations0.init(allocationRange, gpuAccessor->dtos.rotations0);
    allocations.rotations1.init(allocationRange, gpuAccessor->dtos.rotations1);
    allocations.rotations2.init(allocationRange, gpuAccessor->dtos.rotations2);
    allocations.rotations3.init(allocationRange, gpuAccessor->dtos.rotations3);
  }

  void RenderingGroup::setUpDrawable() {
    auto shader = selectShader();
    drawable = new SphereDrawable(
      *shader,
      settings->world.maxActorCount,
      (GLfloat*)allocations.positions.getPtr(),
      (GLfloat*)allocations.rotations0.getPtr(),
      (GLfloat*)allocations.rotations1.getPtr(),
      (GLfloat*)allocations.rotations2.getPtr(),
      (GLfloat*)allocations.rotations3.getPtr(),
      (GLfloat*)allocations.colors.getPtr()
    );
  }

  void RenderingGroup::updateDrawable() {
    drawable->shader = selectShader();
    if (texture == TEXTURE_CHECK && drawable->texture == nullptr) {
      drawable->texture = new alcube::drawing::textures::CheckTexture(128, 128);
    }
    drawable->shape->instanceCount = allocationRange->getAllocatedLength();
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

  void RenderingGroup::updateGPU() {
    input();
    gpuAccessor->kernels.updateDrawingBuffer(
      allocationRange->getAllocatedLength(),
      gpuAccessor->memories.positions,
      gpuAccessor->memories.colors,
      gpuAccessor->memories.rotations0,
      gpuAccessor->memories.rotations1,
      gpuAccessor->memories.rotations2,
      gpuAccessor->memories.rotations3,
      gpuAccessor->memories.actorStates,
      gpuAccessor->memories.renderers,
      allocationRange->getIndex()
    );
  }

  void RenderingGroup::updateHost() {
    auto length = allocationRange->getAllocatedLength();
    auto index = allocationRange->getIndex();
    gpuAccessor->memories.positions.readRange(index, length);
    gpuAccessor->memories.rotations0.readRange(index, length);
    gpuAccessor->memories.rotations1.readRange(index, length);
    gpuAccessor->memories.rotations2.readRange(index, length);
    gpuAccessor->memories.rotations3.readRange(index, length);
    gpuAccessor->memories.colors.readRange(index, length);
    updateDrawable();
  }

  RenderingGroupFactory::RenderingGroupFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::RenderingGroup> *memoryPool,
    alcube::drawing::shaders::Shaders *shaders,
    alcube::models::Settings *settings,
    gpu::GPUAccessor* gpuAccessor,
    Model3DFactory* model3DFactory
  ) {
    this->memoryPool = memoryPool;
    this->shaders = shaders;
    this->settings = settings;
    this->gpuAccessor = gpuAccessor;
    this->model3DFactory = model3DFactory;
  }

  RenderingGroup* RenderingGroupFactory::create(utils::AllocationRange* rendererAllocationRange) {
    auto allocationRange = rendererAllocationRange->allocate(settings->world.maxActorCount);
    auto entity = memoryPool->allocate();
    entity->init(shaders, settings, gpuAccessor, allocationRange, model3DFactory);
    return entity;
  }
}
