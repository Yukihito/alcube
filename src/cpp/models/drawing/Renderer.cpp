#include "Renderer.h"

namespace alcube::models::drawing {
  Renderer::Renderer(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::drawing::Canvas* canvas,
    unsigned int size
  ) {
    this->gpuAccessor = gpuAccessor;
    this->allocationRange = new utils::AllocationRange(0, size);
    this->canvas = canvas;
    renderingGroups = {};
  }

  void Renderer::add(alcube::models::drawing::RenderingGroup *renderingGroup) {
    renderingGroups.push_back(renderingGroup);
  }

  void Renderer::setUp() {
    this->allocate();
    this->update();
    this->registerInCanvas();
  }

  void Renderer::allocate() {
    for (auto renderingGroup : renderingGroups) {
      renderingGroup->allocate(allocationRange, gpuAccessor);
    }
  }

  void Renderer::update() {
    for (auto renderingGroup : renderingGroups) {
      renderingGroup->update();
    }
  }

  void Renderer::registerInCanvas() {
    for (auto renderingGroup : renderingGroups) {
      canvas->add(renderingGroup->getDrawable());
    }
  }
}
