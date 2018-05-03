#ifndef ALCUBE_MODELS_SIMULATOR_H
#define ALCUBE_MODELS_SIMULATOR_H

#include "Actor.h"
#include "physics/softbody/Spring.h"
#include "Alcube.h"
#include "../physics/fluid/Simulator.h"
#include "../physics/softbody/Simulator.h"
#include "drawing/Renderer.h"

namespace alcube::models {
  class Alcube {
    public:
      explicit Alcube(
        alcube::physics::fluid::Simulator* fluidSimulator,
        alcube::physics::softbody::Simulator* softbodySimulator,
        alcube::physics::Simulator* physicsSimulator,
        alcube::models::drawing::Renderer* renderer
      );
      void add(Actor* actor);
      void add(physics::softbody::Spring* spring);
      void add(models::drawing::RenderingGroup* renderingGroup);
      unsigned int getActorCount();
      void setUp();
      void render();
      void update();

    private:
      std::vector<Actor*> actors;
      alcube::physics::fluid::Simulator* fluidSimulator;
      alcube::physics::softbody::Simulator* softbodySimulator;
      alcube::physics::Simulator* physicsSimulator;
      alcube::models::drawing::Renderer* renderer;
  };
}

#endif //ALCUBE_MODELS_SIMULATOR_H
