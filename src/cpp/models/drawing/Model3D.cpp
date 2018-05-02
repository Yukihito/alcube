#include "Model3D.h"


namespace alcube::models::drawing {
  void Model3D::init(alcube::models::drawing::IndexHolder *actorIndexHolder) {
    this->actorIndexHolder = actorIndexHolder;
    color = glm::vec3();
    allocationRange = nullptr;
    allocations = {};
    groupSettings = nullptr;
  }

  void Model3D::allocate(
    alcube::models::drawing::RenderingGroupSettings *groupSettings,
    alcube::utils::AllocationRange *groupAllocationRange,
    alcube::gpu::GPUAccessor *gpuAccessor
  ) {
    allocationRange = groupAllocationRange->allocate(1);
    allocations.colors = new utils::ResourceAllocation<cl_float3>(allocationRange, gpuAccessor->dtos.hostColors);
    allocations.features = new utils::ResourceAllocation<gpu::dtos::Renderer>(allocationRange, gpuAccessor->dtos.hostRenderers);
    this->groupSettings = groupSettings;
  }

  void Model3D::update() {
    allocations.colors->getPtr()[0] = {color.r, color.y, color.z};
    allocations.features->getPtr()->refersToRotations = groupSettings->refersToRotations();
    allocations.features->getPtr()->instanceColorType = groupSettings->getInstanceColorType();
    allocations.features->getPtr()->actorIndex = (unsigned short)actorIndexHolder->getIndex();
  }

  glm::vec3 Model3D::getColor() {
    return color;
  }

  void Model3D::setColor(glm::vec3 v) {
    color = v;
  }

  Model3DFactory::Model3DFactory(alcube::utils::MemoryPool<alcube::models::drawing::Model3D>* memoryPool) {
    this->memoryPool = memoryPool;
  }

  Model3D* Model3DFactory::create(alcube::models::drawing::IndexHolder *actorIndexHolder) {
    auto entity = memoryPool->get();
    entity->init(actorIndexHolder);
    return entity;
  }
}