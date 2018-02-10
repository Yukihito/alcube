#include "FluidSample.h"

namespace alcube::physics::samples::particles {

  FluidSample::FluidSample() : BaseApplication(1600, 1200, 60, "FluidSample") {}

  void FluidSample::onInit() {
    initBase(128, 65536);
    particles = new Particles(shapes, shaders, maxCellCount);
    float w = 128.0f;
    deltaTime *= 2.0f;
    physicsSimulator->gravity *= 1.0f;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(0, 1);
    int size = 52;
    float padding = (size * 2.0f) - (w / 2.0f) - 1.0f;
    int c = 0;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < i + 1; j++) {
        for (int k = 0; k < i + 1; k++) {
          auto cell = new SoftBodyParticle();
          cell->position = -glm::vec3((float)j * 2.0f, (float)i * 2.0f, (float)k * 2.0f) + glm::vec3((w / 2 - 1.0f), padding, (w / 2 - 1.0f)) + 0.01f * glm::vec3(randReal(mt), randReal(mt), randReal(mt));
          cell->mass = (4.0f / 3.0f) * 3.14f * 3.14f * 3.14f * 0.1f;
          physicsSimulator->add(cell);
          cells.push_back(cell);
          particles->colors[c + 0] = (float)j / (float)i;
          particles->colors[c + 1] = (float)i / (float)(size - 1);
          particles->colors[c + 2] = (float)k / (float)i;
          c += 3;
        }
      }
    }
    std::cout << "count: " << (c / 3) << std::endl;
    particles->update(cells);
    drawer->add(particles);
  }

  void FluidSample::onDraw() {
    particles->update(cells);
    BaseApplication::onDraw();
  }
}