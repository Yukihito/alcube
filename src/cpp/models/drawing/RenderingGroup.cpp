#include "RenderingGroup.h"

namespace alcube::models::drawing {
  void RenderingGroup::init(
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

  void RenderingGroup::setUpResources() {
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
    /*
    for (int i = 0; i < childCount; i++) {
      gpuAccessor->memories.hostRenderers.dto[i].instanceColorType = instanceColorType;
      gpuAccessor->memories.hostRenderers.dto[i].refersToRotations = refersToRotations();
    }
     */
    /*
    gpuAccessor->kernels.inputRenderers(
      childCount,
      gpuAccessor->memories.hostRenderers,
      gpuAccessor->memories.renderers,
      gpuAccessor->memories.hostColors,
      gpuAccessor->memories.colors
    );
     */
    drawer->add(drawable);
  }

  bool RenderingGroup::refersToRotations() {
    return texture != TEXTURE_NONE;
  }

  void RenderingGroup::incrementChildCount() {
    childCount++;
  }

  RenderingGroupFactory::RenderingGroupFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::RenderingGroup> *memoryPool,
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

  RenderingGroup* RenderingGroupFactory::create() {
    auto renderer = memoryPool->get();
    renderer->init(gpuAccessor, shaders, drawer, settings);
    return renderer;
  }
}
