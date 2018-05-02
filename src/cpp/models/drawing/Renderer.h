#ifndef ALCUBE_MODELS_RENDERER_H
#define ALCUBE_MODELS_RENDERER_H

#include "RenderingGroup.h"
#include "../../utils/ResourceAllocation.h"
#include "../../drawing/Drawer.h"

namespace alcube::models::drawing {
  class Renderer {
    public:
      explicit Renderer(
        gpu::GPUAccessor* gpuAccessor,
        alcube::drawing::Drawer* drawer,
        unsigned int size
      );
      void add(RenderingGroup* renderingGroup);
      void setUp();

    private:
      void allocate();
      void update();
      void registerInDrawer();
      utils::AllocationRange* allocationRange;
      gpu::GPUAccessor* gpuAccessor;
      alcube::drawing::Drawer* drawer;
      std::vector<RenderingGroup*> renderingGroups;
  };
}

#endif //ALCUBE_MODELS_RENDERER_H
