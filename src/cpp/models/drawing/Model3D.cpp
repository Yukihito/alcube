#include "Model3D.h"


namespace alcube::models::drawing {
  void Model3D::init(
    utils::AllocationRange* actorAllocationRange,
    utils::AllocationRange* allocationRange,
    RenderingGroupSettings* groupSettings,
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->actorAllocationRange = actorAllocationRange;
    this->allocationRange = allocationRange;
    this->groupSettings = groupSettings;
    setUpAllocations(gpuAccessor);
  }

  void Model3D::setUpAllocations(alcube::gpu::GPUAccessor *gpuAccessor) {
    featuresAllocation.init(allocationRange, gpuAccessor->dtos.hostRenderers);
    colorsAllocation.init(allocationRange, gpuAccessor->dtos.hostColors);
    INIT_GPU_BASED_REFERENCE(featuresAllocation, actorIndex, actorAllocationRange);
    INIT_GPU_BASED_PROPERTY(int, featuresAllocation, refersToRotations);
    INIT_GPU_BASED_PROPERTY(unsigned int, featuresAllocation, instanceColorType);
    actorIndex.enableLifeSync();
    if (groupSettings->getInstanceColorType() == INSTANCE_COLOR_TYPE_RANDOM) {
      std::random_device rnd;
      std::mt19937 mt(rnd());
      std::uniform_real_distribution<float> randReal(0, 1);
      colorsAllocation.getPtr()[0] = {randReal(mt), randReal(mt), randReal(mt)};
    } else {
      colorsAllocation.getPtr()[0] = {0.0f, 0.0f, 0.0f};
    }
    refersToRotations.set(groupSettings->refersToRotations());
    instanceColorType.set(groupSettings->getInstanceColorType());
  }

  glm::vec3 Model3D::getColor() {
    auto raw = *colorsAllocation.getPtr();
    return glm::vec3(raw.x, raw.y, raw.z);
  }

  void Model3D::setColor(glm::vec3 v) {
    colorsAllocation.getPtr()[0] = {v.x, v.y, v.z};
  }

  Model3DFactory::Model3DFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::Model3D>* memoryPool,
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->memoryPool = memoryPool;
    this->gpuAccessor = gpuAccessor;
  }

  Model3D* Model3DFactory::create(
    utils::AllocationRange* actorAllocationRange,
    utils::AllocationRange* groupAllocationRange,
    RenderingGroupSettings* groupSettings
  ) {
    auto entity = memoryPool->allocate();
    entity->init(
      actorAllocationRange,
      groupAllocationRange->allocate(1),
      groupSettings,
      gpuAccessor
    );
    return entity;
  }
}