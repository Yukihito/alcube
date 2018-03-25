#ifndef ALCUBE_FLUIDPARTICLE_H
#define ALCUBE_FLUIDPARTICLE_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include "../Actor.h"

namespace alcube::physics::fluid {
  class Actor: public physics::Actor {
    public:
      explicit Actor();
      static std::map<physics::Actor*, Actor*> instances;
      static float density;
      static float stiffness;
      static float viscosity;
      static float mass;
      gpu::dtos::Fluid subPhysicalQuantity;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
