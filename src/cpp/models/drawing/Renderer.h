#ifndef ALCUBE_MODELS_RENDERER_H
#define ALCUBE_MODELS_RENDERER_H

#include "RenderingGroup.h"
#include "../../utils/ResourceAllocation.h"
#include "../../drawing/Canvas.h"

namespace alcube::models::drawing {
  class Renderer {
    public:
      explicit Renderer(
        gpu::GPUAccessor* gpuAccessor,
        alcube::drawing::Canvas* canvas,
        utils::opencl::ResourcesProvider* resourcesProvider,
        unsigned int size
      );
      void add(RenderingGroup* renderingGroup);
      void setUp();
      void update();
      void render();

    private:
      void allocate();
      void initialize();
      void registerInCanvas();
      utils::AllocationRange* allocationRange;
      gpu::GPUAccessor* gpuAccessor;
      alcube::drawing::Canvas* canvas;
      std::vector<RenderingGroup*> renderingGroups;
      utils::opencl::ResourcesProvider* resourcesProvider;
  };
}

#endif //ALCUBE_MODELS_RENDERER_H
