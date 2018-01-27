#include "FluidSample.h"

namespace alcube::physics::samples::particles {

  FluidSample::FluidSample() : BaseApplication(1600, 1200, 60, "FluidSample") {}

  void FluidSample::onInit() {
    initBase(64, 65536);
    int ballCount = 16384;
    physicsSimulator->gravity = 9.8f * 4.0f;
    //int ballCount = 12376;
    //int ballCount = 5626;
    //int ballCount = 2000;
    //int ballCount = 32;
    particles = new Particles(shapes, shaders, maxCellCount);

    float w = 64 - 10.0f;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(-w / 2, w / 2);
    std::uniform_real_distribution<float> randReal2(-10, 10);
    std::uniform_real_distribution<float> randReal3(0, 1);
    for (int i = 0; i < ballCount; i++) {
      auto cell = new Cell();
      cell->mass = 0.1f;
      cell->position = glm::vec3(randReal(mt), randReal(mt), randReal(mt));
      cell->linearMomentum = glm::vec3(5.0f, 0.0f, 0.0f);//glm::vec3(randReal2(mt), randReal2(mt), randReal2(mt));
      cell->elasticity = 0.0f;
      cell->dynamicFrictionCoefficient = 0.0f;
      cell->staticFrictionCoefficient = 0.0f;
      cells.push_back(cell);
      physicsSimulator->add(cell);
      int j = i * 3;
      //particles->colors[j + 0] = randReal3(mt);//(float)(fabs(cell->position.x) + 256.0f) / 512.0f;
      //particles->colors[j + 1] = randReal3(mt);//(float)(fabs(cell->position.y) + 256.0f) / 512.0f;
      //particles->colors[j + 2] = randReal3(mt);//(float)(fabs(cell->position.z) + 256.0f) / 512.0f;
      particles->colors[j + 0] = (cell->position.x + w / 2) / w;
      particles->colors[j + 1] = (cell->position.y + w / 2) / w;
      particles->colors[j + 2] = (cell->position.z + w / 2) / w;
    }

    particles->update(cells);
    drawer->add(particles);
  }

  void FluidSample::onDraw() {
    particles->update(cells);
    BaseApplication::onDraw();
  }
}