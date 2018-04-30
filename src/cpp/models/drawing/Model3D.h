#ifndef ALCUBE_MODEL3D_H
#define ALCUBE_MODEL3D_H

#include "../../gpu/GPUAccessor.h"
#include "RenderingGroup.h"

namespace alcube::models::drawing {
  class Model3D {
    public:
      void init(IndexHolder* indexHolder, gpu::GPUAccessor* gpuAccessor, RenderingGroup* parent);
      RenderingGroup* getParent();
      glm::vec3 getColor();
      void setColor(glm::vec3 v);
      void setUp();

    private:
      IndexHolder* indexHolder;
      gpu::GPUAccessor* gpuAccessor;
      RenderingGroup* parent;
  };

  class Model3DFactory {
    public:
      explicit Model3DFactory(utils::MemoryPool<Model3D>* memoryPool, gpu::GPUAccessor* gpuAccessor);
      Model3D* create(IndexHolder* indexHolder, RenderingGroup* parent);

    private:
      utils::MemoryPool<Model3D>* memoryPool;
      gpu::GPUAccessor* gpuAccessor;

  };
}


#endif //ALCUBE_MODEL3D_H
