#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "../Actor.h"
#include <map>

namespace alcube::physics::softbody {
  class Actor : public physics::Actor {
    public:
      utils::GPUBasedProperty<gpu::dtos::SoftBody, float> elasticity;
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned int*> springIndices;
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned char*> springNodeIndices;
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned int> springCount;
      utils::GPUBasedReference<gpu::dtos::SoftBody> actorIndex;
      explicit Actor();
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange,
        physics::Actor** actors,
        physics::softbody::Actor** subActors
      );
      void beforeWrite() override;
    private:
      utils::ResourceAllocation<gpu::dtos::SoftBody> subAllocation;
      physics::softbody::Actor** subActors = nullptr;
      utils::EventHandler<utils::AllocationMoveEvent> moveSubEventHandler = {};
      utils::EventHandler<utils::DeallocationEvent> subDeallocationEventHandler = {};
  };
}
#define INIT_GPU_BASED_SOFTBODY_PROPERTY(typeName, propName, value) { INIT_GPU_BASED_PROPERTY(typeName, subAllocation, propName); (propName).set(value);}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
