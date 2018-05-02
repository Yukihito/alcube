#include "CanvasWithProfiler.h"

namespace alcube::drawing {
  CanvasWithProfiler::CanvasWithProfiler(Camera *camera, utils::Profiler *profiler) : Canvas(camera) {
    this->profiler = profiler;
    profilers.draw = profiler->create("draw");
    profilers.drawAllDrawables = profiler->create("drawAllDrawables");
    profilers.waitVSync = profiler->create("waitVSync");
  }

  void CanvasWithProfiler::draw() {
    profiler->start(profilers.draw);
    Canvas::draw();
    profiler->stop(profilers.draw);
  }

  void CanvasWithProfiler::waitVSync() {
    profiler->start(profilers.waitVSync);
    Canvas::waitVSync();
    profiler->stop(profilers.waitVSync);
  }

  void CanvasWithProfiler::drawAllDrawables() {
    profiler->start(profilers.drawAllDrawables);
    Canvas::drawAllDrawables();
    glFinish();
    profiler->stop(profilers.drawAllDrawables);
  }
}