#ifndef ALCUBE_DRAWING_CANVAS_WITH_PROFILER_H
#define ALCUBE_DRAWING_CANVAS_WITH_PROFILER_H

#include "Canvas.h"
#include "../utils/Profiler.h"

namespace alcube::drawing {
  class Profilers {
    public:
      int draw;
      int waitVSync;
      int drawAllDrawables;
  };

  class CanvasWithProfiler : public Canvas {
    public:
      explicit CanvasWithProfiler(Camera* camera, utils::Profiler *profiler);
      void draw() override;
      void waitVSync() override;

    protected:
      void drawAllDrawables() override;

    private:
      utils::Profiler *profiler;
      Profilers profilers;
  };
}


#endif //ALCUBE_DRAWING_CANVAS_WITH_PROFILER_H
