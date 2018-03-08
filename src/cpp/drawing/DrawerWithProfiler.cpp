#include "DrawerWithProfiler.h"

namespace alcube::drawing {
  DrawerWithProfiler::DrawerWithProfiler(Camera *camera, utils::Profiler *profiler) : Drawer(camera) {
    this->profiler = profiler;
    profilers.draw = profiler->create("draw");
    profilers.drawAllDrawables = profiler->create("drawAllDrawables");
    profilers.waitVSync = profiler->create("waitVSync");
  }

  void DrawerWithProfiler::draw() {
    profiler->start(profilers.draw);
    Drawer::draw();
    profiler->stop(profilers.draw);
  }

  void DrawerWithProfiler::waitVSync() {
    profiler->start(profilers.waitVSync);
    Drawer::waitVSync();
    profiler->stop(profilers.waitVSync);
  }

  void DrawerWithProfiler::drawAllDrawables() {
    profiler->start(profilers.drawAllDrawables);
    Drawer::drawAllDrawables();
    glFinish();
    profiler->stop(profilers.drawAllDrawables);
  }
}