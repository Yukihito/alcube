#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "../Actor.h"
#include <map>

namespace alcube::physics::softbody {
  class Actor : public physics::Actor {
    public:
      static std::map<physics::Actor*, Actor*> instances;
      explicit Actor() = default;
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange
      );
      gpu::dtos::SoftBody* getSubPhysicalQuantity();
      unsigned short getIndex() override;
    private:
      utils::ResourceAllocation<gpu::dtos::SoftBody>* hostSubPhysicalQuantity = nullptr;
      utils::ResourceAllocation<gpu::dtos::SoftBody>* subPhysicalQuantity = nullptr;
      utils::AllocationRange* subAllocationRange = nullptr;
      void updateIndex() override;
  };
}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
