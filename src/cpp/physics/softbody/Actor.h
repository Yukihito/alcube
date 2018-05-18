#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "../Actor.h"
#include <map>

namespace alcube::physics::softbody {
  class Actor : public physics::Actor {
    public:
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange
      ) override;
      gpu::dtos::SoftBody* getSubState();
      void beforeWrite() override;
    private:
      utils::ResourceAllocation<gpu::dtos::SoftBody> hostSubState = {};
      utils::ResourceAllocation<gpu::dtos::SoftBody> subState = {};
      void updateIndex() override;
  };
}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
