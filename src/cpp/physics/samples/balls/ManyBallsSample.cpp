#include "ManyBallsSample.h"

namespace alcube::physics::samples::balls {
  ManyBallsSample::ManyBallsSample() : BaseApplication(1600, 1200, 60, "BallsSample") {}

  void ManyBallsSample::add(ColorBall *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void ManyBallsSample::add(DrawableFluidParticle *particle) {
    drawer->add(particle);
    physicsSimulator->add(particle);
  }

  void ManyBallsSample::onInit() {
    initBase(64, 16384);
    int ballCount = 16384;
    //int ballCount = 12376;
    //int ballCount = 5626;
    //int ballCount = 2000;
    //int ballCount = 32;
    float w = 64;
    deltaTime *= 2.0f;
    physicsSimulator->gravity *= 2.0f;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    //std::uniform_real_distribution<float> randReal(-w / 2, w / 2);
    //std::uniform_real_distribution<float> randReal2(-10, 10);
    std::uniform_real_distribution<float> randReal3(0, 1);
    for (int i = 0; i < 32; i++) {
      for (int j = 0; j < i ; j++) {
        for (int k = 0; k < i; k++) {
          auto particle = new DrawableFluidParticle(shapes, shaders, glm::vec3(randReal3(mt) * 1.0f, randReal3(mt) * 1.0f, randReal3(mt)));
          particle->position = glm::vec3((float)i * 2.0f, (float)j * 2.0f, (float)k * 2.0f) - glm::vec3(w/2 - 1.0f, w/2 - 1.0f, w/2 - 1.0f) + 0.01f * glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt));
//          particle->mass = (4.0f / 3.0f) * 3.14f * 3.14f * 3.14f * 0.1f;
          add(particle);
        }
      }
    }
    /*
    bool showsLinerMomentum = false;
    for (int i = 0; i < ballCount; i++) {
      auto ball = new ColorBall(shapes, shaders, glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt)),
                                showsLinerMomentum);
      ball->position = glm::vec3(randReal(mt) / 2, randReal(mt), randReal(mt) / 2);
      //ball->linearMomentum = glm::vec3(randReal2(mt), randReal2(mt), randReal2(mt));
      ball->elasticity = 0.8f;
      ball->mass = (4.0f / 3.0f) * 3.14f * 3.14f * 3.14f * 0.1f;
      add(ball);
    }
     */
  }
}