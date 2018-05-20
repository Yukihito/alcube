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
      static float density;
      static float stiffness;
      static float viscosity;
      static float mass;
      utils::GPUBasedReference<gpu::dtos::Fluid> actorIndex;
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange,
        physics::Actor** actors,
        physics::fluid::Actor** subEntities
      );
      void beforeWrite() override;
    private:
      physics::fluid::Actor** subEntities = nullptr;
      utils::ResourceAllocation<gpu::dtos::Fluid> subAllocation;
      utils::EventHandler<utils::AllocationMoveEvent> moveSubEventHandler = {};
      utils::EventHandler<utils::DeallocationEvent> subDeallocationEventHandler = {};
      gpu::GPUAccessor* gpuAccessor = nullptr;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
