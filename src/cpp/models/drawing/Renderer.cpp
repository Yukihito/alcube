#include "Renderer.h"

namespace alcube::models::drawing {
  Renderer::Renderer(
    alcube::gpu::GPUAccessor *gpuAccessor,
    alcube::drawing::Canvas* canvas,
    utils::opencl::ResourcesProvider* resourcesProvider,
    unsigned int size
  ) {
    this->gpuAccessor = gpuAccessor;
    this->allocationRange = new utils::AllocationRange(0, size);
    this->canvas = canvas;
    this->resourcesProvider = resourcesProvider;
    renderingGroups = {};
  }

  void Renderer::add(alcube::models::drawing::RenderingGroup *renderingGroup) {
    renderingGroups.push_back(renderingGroup);
  }

  void Renderer::setUp() {
    this->allocate();
    this->initialize();
    this->registerInCanvas();
    unsigned int allocatedLength = allocationRange->getAllocatedLength();
    gpuAccessor->kernels.inputRenderers(
      allocatedLength,
      gpuAccessor->memories.hostRenderers,
      gpuAccessor->memories.renderers,
      gpuAccessor->memories.hostColors,
      gpuAccessor->memories.colors
    );
    gpuAccessor->memories.positions.setCount(allocatedLength);
    gpuAccessor->memories.rotations0.setCount(allocatedLength);
    gpuAccessor->memories.rotations1.setCount(allocatedLength);
    gpuAccessor->memories.rotations2.setCount(allocatedLength);
    gpuAccessor->memories.rotations3.setCount(allocatedLength);
    gpuAccessor->memories.colors.setCount(allocatedLength);
  }

  void Renderer::render() {
    unsigned int allocatedLength = allocationRange->getAllocatedLength();
    // TODO: start profiling
    gpuAccessor->memories.positions.setCount(allocatedLength);
    gpuAccessor->memories.positions.read();
    gpuAccessor->memories.rotations0.setCount(allocatedLength);
    gpuAccessor->memories.rotations0.read();
    gpuAccessor->memories.rotations1.setCount(allocatedLength);
    gpuAccessor->memories.rotations1.read();
    gpuAccessor->memories.rotations2.setCount(allocatedLength);
    gpuAccessor->memories.rotations2.read();
    gpuAccessor->memories.rotations3.setCount(allocatedLength);
    gpuAccessor->memories.rotations3.read();
    gpuAccessor->memories.colors.setCount(allocatedLength);
    gpuAccessor->memories.colors.read();
    // TODO: stop profiling
    canvas->draw();
  }

  void Renderer::update() {
    gpuAccessor->kernels.updateDrawingBuffer(
      allocationRange->getAllocatedLength(),
      gpuAccessor->memories.positions,
      gpuAccessor->memories.colors,
      gpuAccessor->memories.rotations0,
      gpuAccessor->memories.rotations1,
      gpuAccessor->memories.rotations2,
      gpuAccessor->memories.rotations3,
      gpuAccessor->memories.physicalQuantities,
      gpuAccessor->memories.renderers
    );
    clFinish(resourcesProvider->queue->queue);
  }

  void Renderer::allocate() {
    for (auto renderingGroup : renderingGroups) {
      renderingGroup->allocate(allocationRange, gpuAccessor);
    }
  }

  void Renderer::initialize() {
    for (auto renderingGroup : renderingGroups) {
      renderingGroup->initialize();
    }
  }

  void Renderer::registerInCanvas() {
    for (auto renderingGroup : renderingGroups) {
      canvas->add(renderingGroup->getDrawable());
    }
  }
}
