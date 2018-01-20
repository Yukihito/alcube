#include "ManyBallsSample.h"

namespace alcube::physics::samples::balls {
  ManyBallsSample::ManyBallsSample() : BaseApplication(1600, 1200, 30, "BallsSample") {}

  void ManyBallsSample::add(ColorBall *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void ManyBallsSample::onInit() {
    initBase(64, 16384);
    int ballCount =12376;
    //int ballCount = 5626;
    //int ballCount = 2000;
    //int ballCount = 32;

    float w = 64 - 1.0f;
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
  }
}