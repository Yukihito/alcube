#include "Alcube.h"

namespace alcube::models {
  Alcube::Alcube(
    alcube::physics::fluid::Simulator* fluidSimulator,
    alcube::physics::softbody::Simulator* softbodySimulator,
    alcube::physics::Simulator* physicsSimulator,
    alcube::models::drawing::Renderer* renderer
  ) {
    this->fluidSimulator = fluidSimulator;
    this->softbodySimulator = softbodySimulator;
    this->physicsSimulator = physicsSimulator;
    this->physicsSimulator->add(softbodySimulator);
    this->physicsSimulator->add(fluidSimulator);
    this->renderer = renderer;
  }

  void Alcube::setUp() {
    physicsSimulator->setUp();
  }

  void Alcube::render() {
    renderer->render();
  }

  void Alcube::update() {
    physicsSimulator->update();
    renderer->update();
  }

  void Alcube::gc() {
    physicsSimulator->gc();
    renderer->gc();
  }

  unsigned int Alcube::getActorCount() {
    return physicsSimulator->getActorCount();
  }
}
