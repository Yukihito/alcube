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
      explicit Actor() = default;
      static float density;
      static float stiffness;
      static float viscosity;
      static float mass;
      utils::GPUBasedReference<gpu::dtos::Fluid> actorIndex = {};
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange,
        physics::Actor** actors,
        physics::fluid::Actor** subActors
      );
      void beforeWrite() override;
    private:
      utils::GPUBasedStruct<gpu::dtos::Fluid> subStateStruct = {};
      gpu::GPUAccessor* gpuAccessor = nullptr;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
