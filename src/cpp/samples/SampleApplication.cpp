#include "SampleApplication.h"

namespace alcube::samples {
  SampleApplication::SampleApplication() : ApplicationBase(1600, 1200, 60, "SampleApplication") {}

  void SampleApplication::add(physics::SoftBodyActor *actor) {
    actorCount++;
    physicsSimulator->add(actor);
  }

  void SampleApplication::add(physics::FluidActor *actor) {
    actorCount++;
    physicsSimulator->add(actor);
  }

  void SampleApplication::onInit() {
    beforeSetup(64, 65536);
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
          auto actor = new physics::FluidActor();
          glm::vec3 pos = glm::vec3((float)i * 2.0f, (float)j * 2.0f, (float)k * 2.0f) - glm::vec3(w/2 - 1.0f, w/2 - 1.0f, w/2 - 1.0f) + 0.01f * glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt));
          actor->physicalQuantity.position = { pos.x, pos.y, pos.z };
          add(actor);
        }
      }
    }

    // soft body
    int softBodySize = 12;
    float softBodyEdgeLength = softBodySize * 2.0f;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          auto actor = new physics::SoftBodyActor();
          actor->physicalQuantity.mass = 0.2f;
          glm::vec3 offset = glm::vec3(-w / 4, w / 4, 0.0f);
          glm::vec3 pos = glm::vec3(
            x * 2.0f - (softBodyEdgeLength / 2.0f),
            y * 2.0f - (softBodyEdgeLength / 2.0f),
            z * 2.0f - (softBodyEdgeLength / 2.0f)
          ) + offset;
          actor->physicalQuantity.position = {pos.x, pos.y, pos.z};
          actor->physicalQuantity.linearMomentum = { 3.0f, 1.0f, 0.5f };
          actor->subPhysicalQuantity.elasticity = 0.0f;
          add(actor);
        }
      }
    }

    unsigned int index = 0;
    float k = 64.0f;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          if (x + 1 < softBodySize) {
            auto springX = new physics::Spring();
            springX->nodes[0].particle = physicsSimulator->getSoftBodyActor(index);
            springX->nodes[0].position = glm::vec3(1.0f, 0.0f, 0.0f);
            springX->nodes[1].particle = physicsSimulator->getSoftBodyActor(index + 1);
            springX->nodes[1].position = glm::vec3(-1.0f, 0.0f, 0.0f);
            springX->k = k;
            physicsSimulator->add(springX);
          }

          if (y + 1 < softBodySize) {
            auto springY = new physics::Spring();
            springY->nodes[0].particle = physicsSimulator->getSoftBodyActor(index);
            springY->nodes[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
            springY->nodes[1].particle = physicsSimulator->getSoftBodyActor(index + softBodySize);
            springY->nodes[1].position = glm::vec3(0.0f, -1.0f, 0.0f);
            springY->k = k;
            physicsSimulator->add(springY);
          }

          if (z + 1 < softBodySize) {
            auto springZ = new physics::Spring();
            springZ->nodes[0].particle = physicsSimulator->getSoftBodyActor(index);
            springZ->nodes[0].position = glm::vec3(0.0f, 0.0f, 1.0f);
            springZ->nodes[1].particle = physicsSimulator->getSoftBodyActor(index + softBodySize * softBodySize);
            springZ->nodes[1].position = glm::vec3(0.0f, 0.0f, -1.0f);
            springZ->k = k;
            physicsSimulator->add(springZ);
          }
          index++;
        }
      }
    }

    glm::vec3 color = glm::vec3(0.4f, 0.4f, 1.0f);
    drawable = new SphereDrawable(
      shaders->directionalLight,
      color,
      maxCellCount,
      (GLfloat*)gpuAccessor->memories.positions.dto
    );
    drawable->shape->instanceCount = actorCount;
    drawer->add(drawable);
    afterSetup();
  }
}