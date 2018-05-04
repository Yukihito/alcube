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
      static std::map<physics::Actor*, Actor*> instances;
      explicit Actor() = default;
      static float density;
      static float stiffness;
      static float viscosity;
      static float mass;

      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange
      );

      gpu::dtos::Fluid* getSubPhysicalQuantity();
      unsigned short getIndex() override;
      void setIndex(unsigned short index) override;
    private:
      utils::ResourceAllocation<gpu::dtos::Fluid>* hostSubPhysicalQuantity;
      utils::ResourceAllocation<gpu::dtos::Fluid>* subPhysicalQuantity;
      // gpu::dtos::Fluid subPhysicalQuantity;
  };
}

#endif //ALCUBE_FLUIDPARTICLE_H
