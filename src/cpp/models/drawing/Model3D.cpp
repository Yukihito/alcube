#include "Model3D.h"


namespace alcube::models::drawing {
  void Model3D::init(
    alcube::models::drawing::IndexHolder *actorIndexHolder,
    utils::AllocationRange* allocationRange,
    RenderingGroupSettings* groupSettings,
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->actorIndexHolder = actorIndexHolder;
    this->allocationRange = allocationRange;
    this->groupSettings = groupSettings;
    setUpAllocations(gpuAccessor);
  }

  void Model3D::setUpAllocations(alcube::gpu::GPUAccessor *gpuAccessor) {
    allocations = {};
    allocations.colors.init(allocationRange, gpuAccessor->dtos.hostColors);
    allocations.features.init(allocationRange, gpuAccessor->dtos.hostRenderers);

    if (groupSettings->getInstanceColorType() == INSTANCE_COLOR_TYPE_RANDOM) {
      std::random_device rnd;
      std::mt19937 mt(rnd());
      std::uniform_real_distribution<float> randReal(0, 1);
      allocations.colors.getPtr()[0] = {randReal(mt), randReal(mt), randReal(mt)};
    } else {
      allocations.colors.getPtr()[0] = {0.0f, 0.0f, 0.0f};
    }
    allocations.features.getPtr()->refersToRotations = groupSettings->refersToRotations();
    allocations.features.getPtr()->instanceColorType = groupSettings->getInstanceColorType();
    allocations.features.getPtr()->actorIndex = (unsigned short)actorIndexHolder->getIndex();
  }

  glm::vec3 Model3D::getColor() {
    auto raw = allocations.colors.get();
    return glm::vec3(raw.x, raw.y, raw.z);
  }

  void Model3D::setColor(glm::vec3 v) {
    allocations.colors.getPtr()[0] = {v.x, v.y, v.z};
  }

  Model3DFactory::Model3DFactory(
    alcube::utils::MemoryPool<alcube::models::drawing::Model3D>* memoryPool,
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->memoryPool = memoryPool;
    this->gpuAccessor = gpuAccessor;
  }

  Model3D* Model3DFactory::create(
    alcube::models::drawing::IndexHolder *actorIndexHolder,
    utils::AllocationRange* groupAllocationRange,
    RenderingGroupSettings* groupSettings
  ) {
    auto entity = memoryPool->get();
    entity->init(
      actorIndexHolder,
      groupAllocationRange->allocate(1),
      groupSettings,
      gpuAccessor
    );
    return entity;
  }
}