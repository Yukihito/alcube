#ifndef ALCUBE_DRAWERWITHPROFILER_H
#define ALCUBE_DRAWERWITHPROFILER_H

#include "Drawer.h"
#include "../utils/Profiler.h"

namespace alcube::drawing {
  class Profilers {
    public:
      int draw;
      int waitVSync;
      int drawAllDrawables;
  };

  class DrawerWithProfiler : public Drawer {
    public:
      explicit DrawerWithProfiler(Camera* camera, utils::Profiler *profiler);
      void draw() override;
      void waitVSync() override;

    protected:
      void drawAllDrawables() override;

    private:
      utils::Profiler *profiler;
      Profilers profilers;
  };
}


#endif //ALCUBE_DRAWERWITHPROFILER_H
