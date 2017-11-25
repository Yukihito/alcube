#include "BallsSample.h"

namespace alcube::physics::samples::balls {
  Ball::Ball(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders) {
    shape = &shapes->points.sphere;
    shader = &shaders->simple;
  }

  glm::mat4 Ball::getModelMat() {
    return glm::translate(position);
  }

  void BallsSample::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    appName = "BallsSample";
    isMultiSampleEnabled = false;
  }

  void BallsSample::add(Ball *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void BallsSample::onInit() {
    maxCellCount = 16384; // 2^14
    deltaTime = 1.0f / 30.0f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = 64;
    unsigned int yGridCount = 64;
    unsigned int zGridCount = 64;
    float near = 0.1f;
    float far = gridEdgeLength * xGridCount * 2.0f;
    shaders = new drawing::shaders::Shaders(new utils::FileUtil());
    shapes = new drawing::shapes::Shapes();
    camera = new drawing::Camera(
      glm::vec3(0.0f, 0.0f, 20.0f),
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
    profiler = new utils::Profiler();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");

    auto ball0 = new Ball(shapes, shaders);
    ball0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    ball0->linearMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
    auto ball1 = new Ball(shapes, shaders);
    ball1->position = glm::vec3(5.0f, 0.0f, 0.0f);
    ball1->linearMomentum = glm::vec3(-1.0f, 0.1f, 0.0f);
    add(ball0);
    add(ball1);

    profiler->start(profilers.all);
  }

  void BallsSample::onDraw() {
    drawer->draw();
  }

  void BallsSample::onUpdate() {
    profiler->start(profilers.update);
    physicsSimulator->update(deltaTime);
    profiler->stop(profilers.update);

    profiler->stop(profilers.all);

    profiler->update();


    profiler->start(profilers.all);
  }

  void BallsSample::onClose() {
    resources->release();
  }
}