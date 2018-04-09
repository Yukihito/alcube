#include "SampleApplication.h"

namespace alcube::samples {
  SampleApplication::SampleApplication(const char* programName) : ApplicationBase(1600, 1200, 60, "SampleApplication", programName) {}

  void SampleApplication::onInit() {
    beforeSetup(64, 65536);
    float w = 64;
    deltaTime *= 1.0f;
    physicsSimulator->gravity *= 2.0f;
    /*
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(0, 0.5);
    std::uniform_real_distribution<float> randReal3(0, 1);

    // fluid
    auto fluidFeatures = new models::physics::fluid::Features();
    for (int i = 0; i < 32; i++) {
      for (int j = 0; j < i ; j++) {
        for (int k = 0; k < i; k++) {
          glm::vec3 pos = glm::vec3((float)i * 2.0f, (float)j * 2.0f, (float)k * 2.0f) - glm::vec3(w/2 - 1.0f, w/2 - 1.0f, w/2 - 1.0f) + 0.01f * glm::vec3(randReal3(mt), randReal3(mt), randReal3(mt));
          auto actor = actorFactory->create(fluidFeatures);
          actor->setPosition(pos);
          cube->add(actor);
        }
      }
    }

    // soft body
    std::vector<models::Actor*> softBodies = {};
    auto softbodyFeatures = new models::physics::softbody::Features();
    softbodyFeatures->setElasticity(0.0f);
    softbodyFeatures->setMass(0.2f);
    int softBodySize = 12;
    float softBodyEdgeLength = softBodySize * 2.0f;
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          glm::vec3 offset = glm::vec3(-w / 4, w / 4, 0.0f);
          glm::vec3 pos = glm::vec3(
            x * 2.0f - (softBodyEdgeLength / 2.0f),
            y * 2.0f - (softBodyEdgeLength / 2.0f),
            z * 2.0f - (softBodyEdgeLength / 2.0f)
          ) + offset;
          auto actor = actorFactory->create(softbodyFeatures);
          actor->setPosition(pos);
          actor->setLinearMomentum(glm::vec3(3.0f, 1.0f, 0.5f));
          cube->add(actor);
          softBodies.push_back(actor);
        }
      }
    }

    unsigned int index = 0;
    //float k = 64.0f;
    springFactory->setK(64.0f);
    for (int z = 0; z < softBodySize; z++) {
      for (int y = 0; y < softBodySize; y++) {
        for (int x = 0; x < softBodySize; x++) {
          if (x + 1 < softBodySize) {
            auto spring = springFactory->create(
              softBodies[index], glm::vec3(1.0f, 0.0f, 0.0f),
              softBodies[index + 1], glm::vec3(-1.0f, 0.0f, 0.0f)
            );
            cube->add(spring);
          }

          if (y + 1 < softBodySize) {
            auto spring = springFactory->create(
              softBodies[index], glm::vec3(0.0f, 1.0f, 0.0f),
              softBodies[index + softBodySize], glm::vec3(0.0f, -1.0f, 0.0f)
            );
            cube->add(spring);
          }

          if (z + 1 < softBodySize) {
            auto spring = springFactory->create(
              softBodies[index], glm::vec3(0.0f, 0.0f, 1.0f),
              softBodies[index + softBodySize * softBodySize], glm::vec3(0.0f, 0.0f, -1.0f)
            );
            cube->add(spring);
          }
          index++;
        }
      }
    }
     */

    glm::vec3 color = glm::vec3(0.4f, 0.4f, 1.0f);
    drawable = new SphereDrawable(
      shaders->directionalLight,
      color,
      maxCellCount,
      (GLfloat*)gpuAccessor->memories.positions.dto
    );
    drawable->shape->instanceCount = cube->getActorCount();
    drawer->add(drawable);
    afterSetup();
  }
}