#include "ApplicationBase.h"

namespace alcube::physics::samples {
  ApplicationBase::ApplicationBase(
    unsigned int windowWidth,
    unsigned int windowHeight,
    unsigned int fps,
    const std::string& appName
  ) : OpenGLApplication(windowWidth, windowHeight, fps, appName) {}

  void ApplicationBase::initBase(unsigned int worldSize, unsigned int maxCellCount) {
    printSystemInfo();
    initialized = false;
    this->maxCellCount = maxCellCount;
    deltaTime = 1.0f / (float)fps;
    float gravity = 9.8f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = worldSize / gridEdgeLength;
    unsigned int yGridCount = worldSize / gridEdgeLength;
    unsigned int zGridCount = worldSize / gridEdgeLength;
    float near = 0.1f;
    float far = gridEdgeLength * xGridCount * 4.0f;
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
    fileUtil = new utils::FileUtil();
    resourcesProvider = new utils::opencl::ResourcesProvider(fileUtil, new utils::opencl::Resources());
    gpu = new gpu::GPU(
      resourcesProvider,
      maxCellCount,
      utils::math::powerOf2(maxCellCount),
      maxCellCount * 16,
      xGridCount * yGridCount * zGridCount
    );
    drawer = new drawing::Drawer(camera);
    shaders = new drawing::shaders::Shaders(new utils::FileUtil(), drawer->context);
    physicsSimulator = new Simulator(
      maxCellCount,
      gridEdgeLength,
      xGridCount,
      yGridCount,
      zGridCount,
      deltaTime,
      gpu
    );
    physicsSimulator->gravity = gravity;
    profiler = new utils::Profiler();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profilers.draw = profiler->create("draw");
    profilers.drawActors = profiler->create("drawActors");
    profilers.updateDrawable = profiler->create("updateDrawable");
    profiler->start(profilers.all);
  }

  void ApplicationBase::onDraw() {
    drawer->waitVSync();
    profiler->start(profilers.draw);
    profiler->start(profilers.updateDrawable);
    gpu->memories.positions.setCount(physicsSimulator->actorCount);
    gpu->memories.positions.read();
    profiler->stop(profilers.updateDrawable);
    profiler->start(profilers.drawActors);
    drawer->draw();
    profiler->stop(profilers.drawActors);
    profiler->stop(profilers.draw);
  }

  void ApplicationBase::onUpdate() {
    if (!initialized) {
      physicsSimulator->input();
      gpu->memories.positions.setCount(physicsSimulator->actorCount);
      initialized = true;
    }
    profiler->start(profilers.update);
    physicsSimulator->update();
    gpu->kernels.outputPositions(
      physicsSimulator->actorCount,
      gpu->memories.positions,
      gpu->memories.physicalQuantities
    );
    clFinish(resourcesProvider->queue->queue);

    //physicsSimulator->output(); // TODO: delete
    profiler->stop(profilers.update);
    profiler->stop(profilers.all);
    profiler->update();
    profiler->start(profilers.all);
  }

  void ApplicationBase::onClose() {
    resourcesProvider->resources->release();
  }


}
