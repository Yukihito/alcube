#ifndef ALCUBE_DRAWER_H
#define ALCUBE_DRAWER_H

#include "Camera.h"
#include "Drawable.h"
#include "Drawable.h"
#include "../utils/FileUtil.h"
#include "../gpu/GPU.h"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <GL/glut.h>
#include <CL/cl_platform.h>
#include "MultiShape.h"

namespace alcube::drawing {
  class Drawer {
    public:
      explicit Drawer(
        Camera* camera,
        std::mutex* drawablesMutex,
        gpu::GPU* gpu
      );
      void add(Drawable* drawable);
      void waitVSync();
      void draw();
      void updateDrawableBuffers();
      void setUpMultiDrawables();
      void updateMultiDrawables();
      void transformMultiDrawables();
    private:
      gpu::Kernels kernels;
      gpu::Memories memories;
      std::mutex* drawablesMutex;
      int drawableBufferIndex[1];
      Camera* camera;
      std::mutex drawablesQueueMutex;
      std::vector<Drawable*> drawablesQueue;
      std::vector<Drawable*> multiDrawables;
      std::unordered_map<Shader*, std::unordered_map<Shape*, std::vector<Drawable*>*>*> drawables;
      void addInternal(Drawable* drawable);
  };
}

#endif //ALCUBE_DRAWER_H
