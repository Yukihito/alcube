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
    this->actors = {};
    this->renderer = renderer;
  }

  void Alcube::add(Actor *actor) {
    actors.push_back(actor);
    physicsSimulator->add(actor->getPhysicsActor());
  }

  void Alcube::add(physics::softbody::Spring *spring) {
    softbodySimulator->add(spring->getUnderlying());
  }

  void Alcube::add(alcube::models::drawing::RenderingGroup *renderingGroup) {
    renderer->add(renderingGroup);
  }

  void Alcube::setUpRenderer() {
    renderer->setUp();
  }

  unsigned int Alcube::getActorCount() {
    return physicsSimulator->actorCount;
  }
}
