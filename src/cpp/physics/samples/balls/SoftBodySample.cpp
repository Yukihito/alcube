#include "SoftBodySample.h"

namespace alcube::physics::samples::balls {
  SoftBodySample::SoftBodySample() : BaseApplication(1600, 1200, 60, "SoftBodySample") {}

  void SoftBodySample::add(ColorBall *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void SoftBodySample::onInit() {
    initBase(64, 65536);
    int softBodySize = 12;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal3(0, 1);
    std::uniform_real_distribution<float> randReal(0, 0.5);
    float softBodyEdgeLength = softBodySize * 2.0f;
    bool showsLinearMomentum = false;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          auto ball = new ColorBall(shapes, shaders, glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt)), showsLinearMomentum);
          ball->mass = 0.2f;
          ball->position = glm::vec3(
            x * 2.0f - (softBodyEdgeLength / 2.0f),
            y * 2.0f - (softBodyEdgeLength / 2.0f),
            z * 2.0f - (softBodyEdgeLength / 2.0f)
          );
          ball->linearMomentum = glm::vec3(5.0f, 0.0f, 3.0f);
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
            springX->nodes[0].particle = physicsSimulator->getSoftBodyParticle(index);
            springX->nodes[0].position = glm::vec3(1.0f, 0.0f, 0.0f);
            springX->nodes[1].particle = physicsSimulator->getSoftBodyParticle(index + 1);
            springX->nodes[1].position = glm::vec3(-1.0f, 0.0f, 0.0f);
            springX->k = k;
            physicsSimulator->add(springX);
          }

          if (y + 1 < softBodySize) {
            auto springY = new Spring();
            springY->nodes[0].particle = physicsSimulator->getSoftBodyParticle(index);
            springY->nodes[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
            springY->nodes[1].particle = physicsSimulator->getSoftBodyParticle(index + softBodySize);
            springY->nodes[1].position = glm::vec3(0.0f, -1.0f, 0.0f);
            springY->k = k;
            physicsSimulator->add(springY);
          }

          if (z + 1 < softBodySize) {
            auto springZ = new Spring();
            springZ->nodes[0].particle = physicsSimulator->getSoftBodyParticle(index);
            springZ->nodes[0].position = glm::vec3(0.0f, 0.0f, 1.0f);
            springZ->nodes[1].particle = physicsSimulator->getSoftBodyParticle(index + softBodySize * softBodySize);
            springZ->nodes[1].position = glm::vec3(0.0f, 0.0f, -1.0f);
            springZ->k = k;
            physicsSimulator->add(springZ);
          }
          index++;
        }
      }
    }

  }
}