#include "SoftBodyAndFluidInteraction.h"

namespace alcube::physics::samples::balls {
  SoftBodyAndFluidInteraction::SoftBodyAndFluidInteraction() : ApplicationBase(1600, 1200, 60, "SoftBodyAndFluidInteraction") {}

  void SoftBodyAndFluidInteraction::add(SoftBodyParticle *ball) {
    actorCount++;
    physicsSimulator->add(ball);
  }

  void SoftBodyAndFluidInteraction::add(FluidParticle *particle) {
    actorCount++;
    physicsSimulator->add(particle);
  }

  void SoftBodyAndFluidInteraction::onInit() {
    initBase(64, 65536);
    float w = 64;
    deltaTime *= 1.0f;
    physicsSimulator->gravity *= 2.0f;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(0, 0.5);
    std::uniform_real_distribution<float> randReal3(0, 1);

    // fluid
    for (int i = 0; i < 32; i++) {
      for (int j = 0; j < i ; j++) {
        for (int k = 0; k < i; k++) {
          auto particle = new FluidParticle();
          particle->position = glm::vec3((float)i * 2.0f, (float)j * 2.0f, (float)k * 2.0f) - glm::vec3(w/2 - 1.0f, w/2 - 1.0f, w/2 - 1.0f) + 0.01f * glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt));
          add(particle);
        }
      }
    }

    // soft body
    int softBodySize = 12;
    float softBodyEdgeLength = softBodySize * 2.0f;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          auto ball = new SoftBodyParticle();
          ball->mass = 0.2f;
          glm::vec3 offset = glm::vec3(-w / 4, w / 4, 0.0f);
          ball->position = glm::vec3(
            x * 2.0f - (softBodyEdgeLength / 2.0f),
            y * 2.0f - (softBodyEdgeLength / 2.0f),
            z * 2.0f - (softBodyEdgeLength / 2.0f)
          ) + offset;
          ball->linearMomentum = glm::vec3(3.0f, 1.0f, 0.5f);
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

    glm::vec3 color = glm::vec3(0.4f, 0.4f, 1.0f);
    colorBalls = new ColorBalls(
      shaders,
      color,
      maxCellCount,
      &gpu->memories.positions
    );
    ((drawing::shapes::triangles::MultiSphere*)colorBalls->shape)->instanceCount = actorCount;
    drawer->add(colorBalls);
  }
}