#ifndef ALCUBE_MODELS_SIMULATOR_H
#define ALCUBE_MODELS_SIMULATOR_H

#include "actor/Actor.h"
#include "physics/softbody/Spring.h"
#include "Alcube.h"
#include "../physics/fluid/Simulator.h"
#include "../physics/softbody/Simulator.h"

namespace alcube::models {
  class Alcube {
    public:
      explicit Alcube(
        alcube::physics::fluid::Simulator* fluidSimulator,
        alcube::physics::softbody::Simulator* softbodySimulator,
        alcube::physics::Simulator* physicsSimulator
      );
      void add(actor::Actor* actor);
      void add(physics::softbody::Spring* spring);
      unsigned int getActorCount();

    private:
      alcube::physics::fluid::Simulator* fluidSimulator;
      alcube::physics::softbody::Simulator* softbodySimulator;
      alcube::physics::Simulator* physicsSimulator;
  };
}

#endif //ALCUBE_MODELS_SIMULATOR_H
