#include "Alcube.h"

namespace alcube::models {
  Alcube::Alcube(
    alcube::physics::fluid::Simulator* fluidSimulator,
    alcube::physics::softbody::Simulator* softbodySimulator,
    alcube::physics::Simulator* physicsSimulator
  ) {
    this->fluidSimulator = fluidSimulator;
    this->softbodySimulator = softbodySimulator;
    this->physicsSimulator = physicsSimulator;
    physicsSimulator->add(softbodySimulator);
    physicsSimulator->add(fluidSimulator);
  }

  void Alcube::add(actor::Actor *actor) {
    physicsSimulator->add(actor->getPhysicsActor());
  }

  void Alcube::add(physics::softbody::Spring *spring) {
    softbodySimulator->add(spring->getUnderlying());
  }

  unsigned int Alcube::getActorCount() {
    return physicsSimulator->actorCount;
  }
}
