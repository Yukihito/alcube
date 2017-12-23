#include "ManyBallsSample.h"

namespace alcube::physics::samples::balls {
  ColorBall::ColorBall(
    drawing::shapes::Shapes *shapes,
    drawing::shaders::Shaders *shaders,
    glm::vec3 diffuse
  ) {
    shape = &shapes->triangles.sphere;
    shader = &shaders->tile;
    material = new drawing::Material();
    material->diffuse = diffuse;
    material->ambient = diffuse / 2.0f;
    material->specular = glm::vec3(0.3f, 0.3f, 0.3f);
  }

  glm::mat4 ColorBall::getModelMat() {
    return glm::translate(position) * glm::toMat4(rotation) * glm::scale(glm::vec3(radius, radius, radius));
  }

  drawing::Material* ColorBall::getMaterial() {
    bool showLinearMomentum = false;
    if(showLinearMomentum) {
      /*
      glm::vec3 diffuse = glm::vec3(
        sqrtf(linearMomentum.y * linearMomentum.y) / 15.0f,
        0.0f,
        0.0f);
      if (linearMomentum.y > 0.0f) {
        diffuse = glm::vec3(
          0.0f,
          sqrtf(linearMomentum.y * linearMomentum.y) / 15.0f,
          0.0f);
      }
       */

      glm::vec3 diffuse = glm::vec3(
        sqrtf(linearMomentum.x * linearMomentum.x) / 15.0f,
        sqrtf(linearMomentum.y * linearMomentum.y) / 15.0f,
        sqrtf(linearMomentum.z * linearMomentum.z) / 15.0f);

      material->diffuse = diffuse;
      material->ambient = diffuse / 2.0f;
    }
    return this->material;
  }

  void ManyBallsSample::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    appName = "BallsSample";
    isMultiSampleEnabled = true;
  }

  void ManyBallsSample::add(ColorBall *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void ManyBallsSample::onInit() {
    maxCellCount = 16384; // 2^14
    int ballCount = 128;
    deltaTime = 1.0f / 30.0f;
    float gravity = 9.8f;
    unsigned int gridEdgeLength = 8;
    unsigned int xGridCount = 2;
    unsigned int yGridCount = 2;
    unsigned int zGridCount = 2;
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

    float w = xGridCount * gridEdgeLength - 1.0f;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(-w/2, w/2);
    std::uniform_real_distribution<float> randReal2(-10, 10);
    std::uniform_real_distribution<float> randReal3(0, 1);

    for (int i = 0; i < ballCount; i++) {
      auto ball = new ColorBall(shapes, shaders, glm::vec3(
        randReal3(mt),
        randReal3(mt),
        randReal3(mt)
      ));
      ball->position = glm::vec3(
        randReal(mt),
        randReal(mt),
        randReal(mt)
      );
      ball->linearMomentum = glm::vec3(
        randReal2(mt),
        randReal2(mt),
        randReal2(mt)
      );
      ball->elasticity = 0.8f;
      add(ball);
    }

    profiler->start(profilers.all);
  }

  void ManyBallsSample::onDraw() {
    drawer->draw();
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
    resources->release();
  }
}