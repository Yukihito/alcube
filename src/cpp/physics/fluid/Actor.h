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

      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange
      ) override;

      gpu::dtos::Fluid* getSubState();
      void beforeWrite() override;
    private:
      utils::ResourceAllocation<gpu::dtos::Fluid> hostSubState = {};
      utils::ResourceAllocation<gpu::dtos::Fluid> subState = {};
      gpu::GPUAccessor* gpuAccessor = nullptr;
      void updateIndex() override;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
