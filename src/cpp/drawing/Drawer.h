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
#include <CL/cl_platform.h>

namespace alcube::drawing {
  class Drawer {
    public:
      explicit Drawer(Camera* camera);
      void add(Drawable* drawable);
      void waitVSync();
      void draw();
      Context context;
    private:
      Camera* camera;
      std::mutex drawablesQueueMutex;
      std::vector<Drawable*> drawablesQueue;
      std::unordered_map<Shader*, std::vector<Drawable*>*> shaderToDrawables;
      void addInternal(Drawable* drawable);
      void addWaitingDrawables();
      void initContext();
      void drawAllDrawables();
  };
}

#endif //ALCUBE_DRAWER_H
