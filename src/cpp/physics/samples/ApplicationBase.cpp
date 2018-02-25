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
    fileUtil = new utils::FileUtil();
    resourcesProvider = new utils::opencl::ResourcesProvider(fileUtil, new utils::opencl::Resources());
    uint modelVertexCount = shapes->triangles.sphere.buffer->vbos.vertices->allocationSize / sizeof(GLfloat);
    auto gpu = new gpu::GPU(
      resourcesProvider,
      maxCellCount,
      utils::math::powerOf2(maxCellCount),
      maxCellCount * 16,
      xGridCount * yGridCount * zGridCount,
      modelVertexCount,
      modelVertexCount * maxCellCount
    );
    drawer = new drawing::Drawer(camera, &mutex, gpu);
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
    profiler->start(profilers.all);
  }

  void ApplicationBase::onDraw() {
    profiler->start(profilers.draw);
    drawer->draw();
    profiler->stop(profilers.draw);
  }

  void ApplicationBase::onUpdate() {
    if (!initialized) {
      physicsSimulator->input();
      initialized = true;
    }
    profiler->start(profilers.update);
    physicsSimulator->update();
    physicsSimulator->output();
    drawer->updateDrawableBuffers();
    profiler->stop(profilers.update);
    profiler->stop(profilers.all);
    profiler->update();
    profiler->start(profilers.all);
  }

  void ApplicationBase::onClose() {
    resourcesProvider->resources->release();
  }


}
