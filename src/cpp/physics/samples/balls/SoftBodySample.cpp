#include "SoftBodySample.h"

namespace alcube::physics::samples::balls {
  void SoftBodySample::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    appName = "SoftBodySample";
    isMultiSampleEnabled = true;
  }

  void SoftBodySample::add(ColorBall *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void SoftBodySample::onInit() {
    printSystemInfo();
    maxCellCount = 16384; // 2^14
    int softBodySize = 12;
    deltaTime = 1.0f / 30.0f;
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
    //physicsSimulator->sphericalShellRadius = ((gridEdgeLength * xGridCount) / 2.0f) - 1.0f;
    profiler = new utils::Profiler();
    profiler->setShowInterval(1000);
    profiler->enabled = true;
    profilers.update = profiler->create("update");
    profilers.all = profiler->create("all");
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal3(0, 1);
    std::uniform_real_distribution<float> randReal(0, 0.5);
    float softBodyEdgeLength = softBodySize * 2.0f;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          auto ball = new ColorBall(shapes, shaders, glm::vec3(
            //1.0f, 0.4f, 0.4f
            randReal3(mt),
            randReal3(mt),
            randReal3(mt)
          ));
          ball->mass = 0.2f;
          ball->position = glm::vec3(
            x * 2.0f - (softBodyEdgeLength / 2.0f),
            y * 2.0f - (softBodyEdgeLength / 2.0f),
            z * 2.0f - (softBodyEdgeLength / 2.0f)
          );
          ball->linearMomentum = glm::vec3(
            /*randReal(mt)*/5.0f,
            /*randReal(mt)*/0.0f,
            /*randReal(mt)*/3.0f
          );
          ball->elasticity = 0.0f;
          add(ball);
        }
      }
    }

    unsigned int index = 0;
    float k = 64.0f;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          if (x + 1 < softBodySize) {
            auto springX = new Spring();
            springX->nodes[0].cell = physicsSimulator->getCell(index);
            springX->nodes[0].position = glm::vec3(1.0f, 0.0f, 0.0f);
            springX->nodes[1].cell = physicsSimulator->getCell(index + 1);
            springX->nodes[1].position = glm::vec3(-1.0f, 0.0f, 0.0f);
            springX->k = k;
            physicsSimulator->add(springX);
          }

          if (y + 1 < softBodySize) {
            auto springY = new Spring();
            springY->nodes[0].cell = physicsSimulator->getCell(index);
            springY->nodes[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
            springY->nodes[1].cell = physicsSimulator->getCell(index + softBodySize);
            springY->nodes[1].position = glm::vec3(0.0f, -1.0f, 0.0f);
            springY->k = k;
            physicsSimulator->add(springY);
          }

          if (z + 1 < softBodySize) {
            auto springZ = new Spring();
            springZ->nodes[0].cell = physicsSimulator->getCell(index);
            springZ->nodes[0].position = glm::vec3(0.0f, 0.0f, 1.0f);
            springZ->nodes[1].cell = physicsSimulator->getCell(index + softBodySize * softBodySize);
            springZ->nodes[1].position = glm::vec3(0.0f, 0.0f, -1.0f);
            springZ->k = k;
            physicsSimulator->add(springZ);
          }
          index++;
        }
      }
    }

    profiler->start(profilers.all);
  }

  void SoftBodySample::onDraw() {
    drawer->draw();
  }

  void SoftBodySample::onUpdate() {
    profiler->start(profilers.update);
    physicsSimulator->update(deltaTime);
    profiler->stop(profilers.update);

    profiler->stop(profilers.all);

    profiler->update();

    profiler->start(profilers.all);
  }

  void SoftBodySample::onClose() {
    physicsSimulator->tearDownMemories();
    resources->release();
  }
}