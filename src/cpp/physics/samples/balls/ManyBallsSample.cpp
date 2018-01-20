#include "ManyBallsSample.h"

namespace alcube::physics::samples::balls {
  ManyBallsSample::ManyBallsSample() : OpenGLApplication(1600, 1200, 30, "BallsSample") {}

  void ManyBallsSample::add(ColorBall *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void ManyBallsSample::onInit() {
    printSystemInfo();
    maxCellCount = 16384; // 2^14
    int ballCount =12376;
    //int ballCount = 2000;
    //int ballCount = 32;
    deltaTime = 1.0f / (float)fps;
    float gravity = 9.8f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = 8;
    unsigned int yGridCount = 8;
    unsigned int zGridCount = 8;
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
    profiler = new utils::Profiler();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    profilers.draw = profiler->create("draw");

    float w = xGridCount * gridEdgeLength - 1.0f;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(-w/2, w/2);
    std::uniform_real_distribution<float> randReal2(-10, 10);
    std::uniform_real_distribution<float> randReal3(0, 1);
    bool showsLinerMomentum = false;
    for (int i = 0; i < ballCount; i++) {
      auto ball = new ColorBall(shapes, shaders, glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt)), showsLinerMomentum);
      ball->position = glm::vec3(randReal(mt), randReal(mt), randReal(mt));
      ball->linearMomentum = glm::vec3(randReal2(mt), randReal2(mt), randReal2(mt));
      ball->elasticity = 0.8f;
      add(ball);
    }

    profiler->start(profilers.all);
  }

  void ManyBallsSample::onDraw() {
    profiler->start(profilers.draw);
    drawer->draw();
    profiler->stop(profilers.draw);
  }

  void ManyBallsSample::onUpdate() {
    profiler->start(profilers.update);
    physicsSimulator->update(deltaTime);
    profiler->stop(profilers.update);
    profiler->stop(profilers.all);
    profiler->update();
    profiler->start(profilers.all);
  }

  void ManyBallsSample::onClose() {
    physicsSimulator->tearDownMemories();
    resources->release();
  }
}