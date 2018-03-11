#ifndef ALCUBE_DRAWER_H
#define ALCUBE_DRAWER_H

#include "Camera.h"
#include "Drawable.h"
#include "Drawable.h"
#include "../utils/FileUtil.h"
#include "../gpu/GPUAccessor.h"
#include <vector>
#include <unordered_map>
#include <mutex>

namespace alcube::drawing {
  class Drawer {
    public:
      explicit Drawer(Camera* camera);
      void add(Drawable* drawable);
      virtual void draw();
      Context context;
    protected:
      virtual void waitVSync();
      virtual void drawAllDrawables();
    private:
      Camera* camera;
      std::mutex drawablesQueueMutex;
      std::vector<Drawable*> drawablesQueue;
      std::unordered_map<Shader*, std::vector<Drawable*>*> shaderToDrawables;
      void addInternal(Drawable* drawable);
      void addWaitingDrawables();
      void initContext();
  };
}

#endif //ALCUBE_DRAWER_H
