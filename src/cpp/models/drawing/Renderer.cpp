#include "Renderer.h"

namespace alcube::models::drawing {
  void Renderer::init(
    gpu::GPUAccessor* gpuAccessor,
    alcube::drawing::shaders::Shaders* shaders,
    alcube::drawing::Drawer* drawer,
    Settings* settings
  ) {
    this->gpuAccessor = gpuAccessor;
    this->shaders = shaders;
    this->drawer = drawer;
    this->settings = settings;
    material.ambient = glm::vec3();
    material.diffuse = glm::vec3();
    material.specular = glm::vec3();
    drawable = nullptr;
    texture = TEXTURE_NONE;
    instanceColorType = INSTANCE_COLOR_TYPE_NONE;
  }

  glm::vec3 Renderer::getDiffuse() {
    return material.diffuse;
  }

  void Renderer::setDiffuse(glm::vec3 v) {
    material.diffuse = v;
  }

  glm::vec3 Renderer::getAmbient() {
    return material.ambient;
  }

  void Renderer::setAmbient(glm::vec3 v) {
    material.ambient = v;
  }

  glm::vec3 Renderer::getSpecular() {
    return material.specular;
  }

  void Renderer::setSpecular(glm::vec3 v) {
    material.specular = v;
  }

  Texture Renderer::getTexture() {
    return texture;
  }

  void Renderer::setTexture(alcube::models::drawing::Texture v) {
    texture = v;
  }

  InstanceColorType Renderer::getInstanceColorType() {
    return instanceColorType;
  }

  void Renderer::setInstanceColorType(alcube::models::drawing::InstanceColorType v) {
    instanceColorType = v;
  }

  void Renderer::setUpResources() {
    alcube::drawing::Shader* shader = nullptr;
    if (instanceColorType == INSTANCE_COLOR_TYPE_NONE) {
      if (texture == TEXTURE_CHECK) {
        shader = &shaders->directionalLight.textureSingleColor;
      } else {
        shader = &shaders->directionalLight.singleColor;
      }
    } else {
      shader = &shaders->directionalLight.instanceColor;
    }
    drawable = new SphereDrawable(
      *shader,
      material,
      settings->world.maxActorCount,
      (GLfloat*)gpuAccessor->memories.positions.dto,
      (GLfloat*)gpuAccessor->memories.rotations0.dto,
      (GLfloat*)gpuAccessor->memories.rotations1.dto,
      (GLfloat*)gpuAccessor->memories.rotations2.dto,
      (GLfloat*)gpuAccessor->memories.rotations3.dto,
      (GLfloat*)gpuAccessor->memories.colors.dto
    );
    if (texture == TEXTURE_CHECK) {
      drawable->texture = new alcube::drawing::textures::CheckTexture(128, 128);
    }
    drawable->shape->instanceCount = childCount;
    for (int i = 0; i < childCount; i++) {
      gpuAccessor->memories.hostRenderers.dto[i].instanceColorType = instanceColorType;
      gpuAccessor->memories.hostRenderers.dto[i].refersToRotations = refersToRotations();
    }
    gpuAccessor->kernels.inputRenderers(
      childCount,
      gpuAccessor->memories.hostRenderers,
      gpuAccessor->memories.renderers,
      gpuAccessor->memories.hostColors,
      gpuAccessor->memories.colors
    );
    drawer->add(drawable);
  }

  bool Renderer::refersToRotations() {
    return texture != TEXTURE_NONE;
  }

  void Renderer::incrementChildCount() {
    childCount++;
  }

  RendererFactory::RendererFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::Renderer> *memoryPool,
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::drawing::shaders::Shaders *shaders,
    alcube::drawing::Drawer* drawer,
    alcube::models::Settings *settings
  ) {
    this->memoryPool = memoryPool;
    this->gpuAccessor = gpuAccessor;
    this->shaders = shaders;
    this->settings = settings;
    this->drawer = drawer;
  }

  Renderer* RendererFactory::create() {
    auto renderer = memoryPool->get();
    renderer->init(gpuAccessor, shaders, drawer, settings);
    return renderer;
  }

  void InstanceRenderer::init(
    alcube::models::drawing::IndexHolder *indexHolder,
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::models::drawing::Renderer *parent
  ) {
    this->indexHolder = indexHolder;
    this->gpuAccessor = gpuAccessor;
    this->parent = parent;
  }

  glm::vec3 InstanceRenderer::getColor() {
    cl_float3 v = *gpuAccessor->memories.hostColors.at(indexHolder->getIndex());
    return glm::vec3(v.x, v.y, v.z);
  }

  void InstanceRenderer::setColor(glm::vec3 v) {
    gpuAccessor->memories.hostColors.at(indexHolder->getIndex())[0] = {v.x, v.y, v.z};
  }

  Renderer* InstanceRenderer::getParent() {
    return parent;
  }

  InstanceRendererFactory::InstanceRendererFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::InstanceRenderer>* memoryPool,
    alcube::gpu::GPUAccessor *gpuAccessor
  ) {
    this->memoryPool = memoryPool;
    this->gpuAccessor = gpuAccessor;
  }

  InstanceRenderer* InstanceRendererFactory::create(
    alcube::models::drawing::IndexHolder *indexHolder,
    alcube::models::drawing::Renderer *parent) {
    auto renderer = memoryPool->get();
    renderer->init(indexHolder, gpuAccessor, parent);
    parent->incrementChildCount();
    return renderer;
  }
}
