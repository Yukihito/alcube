#ifndef ALCUBE_DRAWER_H
#define ALCUBE_DRAWER_H

#include "Camera.h"
#include "Drawable.h"
#include "../utils/FileUtil.h"
#include "../gpu/GPU.h"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <GL/glut.h>

namespace alcube::drawing {
  class Drawer {
    public:
      explicit Drawer(
        Camera* camera,
        std::mutex* drawablesMutex,
        gpu::GPU* gpu
      );
      void add(Drawable* drawable);
      void draw();
      void updateDrawableBuffers();
    private:
      gpu::Kernels kernels;
      gpu::Memories memories;
      std::mutex* drawablesMutex;
      int drawableBufferIndex[1];
      Camera* camera;
      std::mutex drawablesQueueMutex;
      std::vector<Drawable*> drawablesQueue;
      std::unordered_map<Shader*, std::unordered_map<Shape*, std::vector<Drawable*>*>*> drawables;
      void addInternal(Drawable* drawable);
  };
}

#endif //ALCUBE_DRAWER_H
