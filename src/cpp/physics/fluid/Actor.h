#ifndef ALCUBE_FLUIDPARTICLE_H
#define ALCUBE_FLUIDPARTICLE_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Actor.h"

namespace alcube::physics::fluid {
  class Actor: public physics::Actor {
    public:
      explicit Actor();
      gpu::dtos::Fluid subPhysicalQuantity;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
