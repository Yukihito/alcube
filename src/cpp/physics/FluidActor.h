#ifndef ALCUBE_FLUIDPARTICLE_H
#define ALCUBE_FLUIDPARTICLE_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace alcube::physics {
  class FluidActor {
    public:
      explicit FluidActor();
      glm::vec3 position;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
