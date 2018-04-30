#include "Model3D.h"

namespace alcube::models::drawing {
  void Model3D::init(
    alcube::models::drawing::IndexHolder *indexHolder,
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::models::drawing::RenderingGroup *parent
  ) {
    this->indexHolder = indexHolder;
    this->gpuAccessor = gpuAccessor;
    this->parent = parent;
  }

  void Model3D::setUp() {
    unsigned int i = indexHolder->getIndex();
    gpuAccessor->memories.hostRenderers.dto[i].instanceColorType = parent->getInstanceColorType();
    gpuAccessor->memories.hostRenderers.dto[i].refersToRotations = parent->refersToRotations();
  }

  glm::vec3 Model3D::getColor() {
    cl_float3 v = *gpuAccessor->memories.hostColors.at(indexHolder->getIndex());
    return glm::vec3(v.x, v.y, v.z);
  }

  void Model3D::setColor(glm::vec3 v) {
    gpuAccessor->memories.hostColors.at(indexHolder->getIndex())[0] = {v.x, v.y, v.z};
  }

  RenderingGroup* Model3D::getParent() {
    return parent;
  }

  Model3DFactory::Model3DFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::Model3D>* memoryPool,
    alcube::gpu::GPUAccessor *gpuAccessor
  ) {
    this->memoryPool = memoryPool;
    this->gpuAccessor = gpuAccessor;
  }

  Model3D* Model3DFactory::create(
    alcube::models::drawing::IndexHolder *indexHolder,
    alcube::models::drawing::RenderingGroup *parent) {
    auto renderer = memoryPool->get();
    renderer->init(indexHolder, gpuAccessor, parent);
    parent->incrementChildCount();
    return renderer;
  }
}