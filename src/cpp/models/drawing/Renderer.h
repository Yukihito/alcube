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
        RenderingGroupFactory* renderingGroupFactory,
        unsigned int size
      );
      RenderingGroup* createGroup();
      void update();
      void render();

    private:
      utils::AllocationRange* allocationRange;
      gpu::GPUAccessor* gpuAccessor;
      alcube::drawing::Canvas* canvas;
      std::vector<RenderingGroup*> renderingGroups;
      utils::opencl::ResourcesProvider* resourcesProvider;
      RenderingGroupFactory* renderingGroupFactory;
      utils::MemoryPool<utils::AllocationRange>* allocationRangeMemoryPool;
  };
}

#endif //ALCUBE_MODELS_RENDERER_H
