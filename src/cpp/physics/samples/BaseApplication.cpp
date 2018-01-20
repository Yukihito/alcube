#include "BaseApplication.h"

namespace alcube::physics::samples {
  BaseApplication::BaseApplication(
    unsigned int windowWidth,
    unsigned int windowHeight,
    unsigned int fps,
    const std::string& appName
  ) : OpenGLApplication(windowWidth, windowHeight, fps, appName) {}

  void BaseApplication::initBase(unsigned int worldSize, unsigned int maxCellCount) {
    printSystemInfo();
    this->maxCellCount = maxCellCount;
    deltaTime = 1.0f / (float)fps;
    float gravity = 9.8f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = worldSize / gridEdgeLength;
    unsigned int yGridCount = worldSize / gridEdgeLength;
    unsigned int zGridCount = worldSize / gridEdgeLength;
    float near = 0.1f;
    float far = gridEdgeLength * xGridCount * 4.0f;
    shaders = new drawing::shaders::Shaders(new utils::FileUtil());
    shapes = new drawing::shapes::Shapes();
    camera = new drawing::Camera(
      glm::vec3(0.0f, 0.0f, far / 2.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)windowWidth,
      (float)windowHeight,
      near,
      far
    );
    drawer = new drawing::Drawer(camera, &mutex);
    resources = new utils::opencl::Resources();
    fileUtil = new utils::FileUtil();
    physicsSimulator = new Simulator(
      resources,
      fileUtil,
      &mutex,
      maxCellCount,
      gridEdgeLength,
      xGridCount,
      yGridCount,
      zGridCount
    );
    physicsSimulator->gravity = gravity;
    //physicsSimulator->sphericalShellRadius = ((gridEdgeLength * xGridCount) / 2.0f) - 1.0f;
    profiler = new utils::Profiler();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profilers.draw = profiler->create("draw");
    profiler->start(profilers.all);
  }

  void BaseApplication::onDraw() {
    profiler->start(profilers.draw);
    drawer->draw();
    profiler->stop(profilers.draw);
  }

  void BaseApplication::onUpdate() {
    profiler->start(profilers.update);
    physicsSimulator->update(deltaTime);
    profiler->stop(profilers.update);
    profiler->stop(profilers.all);
    profiler->update();
    profiler->start(profilers.all);
  }

  void BaseApplication::onClose() {
    physicsSimulator->tearDownMemories();
    resources->release();
  }


}
