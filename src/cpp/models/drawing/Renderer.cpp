#include "Renderer.h"

namespace alcube::models::drawing {
  Renderer::Renderer(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::drawing::Canvas* canvas,
    utils::opencl::ResourcesProvider* resourcesProvider,
    RenderingGroupFactory* renderingGroupFactory,
    unsigned int size
  ) {
    this->gpuAccessor = gpuAccessor;
    this->allocationRangeMemoryPool = new utils::MemoryPool<utils::AllocationRange>(size * 4);
    this->allocationRange = allocationRangeMemoryPool->allocate();
    this->allocationRange->init(0, size, allocationRangeMemoryPool);
    this->canvas = canvas;
    this->resourcesProvider = resourcesProvider;
    this->renderingGroupFactory = renderingGroupFactory;
    renderingGroups = {};
  }

  RenderingGroup* Renderer::createGroup() {
    auto entity = renderingGroupFactory->create(allocationRange);
    renderingGroups.push_back(entity);
    canvas->add(entity->getDrawable());
    return entity;
  }

  void Renderer::render() {
    for (auto renderingGroup : renderingGroups) {
      renderingGroup->updateHost();
    }
    canvas->draw();
  }

  void Renderer::update() {
    for (auto renderingGroup : renderingGroups) {
      renderingGroup->updateGPU();
    }
    clFinish(resourcesProvider->queue->queue);
  }
}
