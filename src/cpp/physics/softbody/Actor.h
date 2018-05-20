#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "../Actor.h"
#include <map>

namespace alcube::physics::softbody {
  class Actor: public physics::Actor {
    public:
      utils::GPUBasedProperty<gpu::dtos::SoftBody, float> elasticity;
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned char*> springNodeIndices;
      utils::GPUBasedReference<gpu::dtos::SoftBody> springIndices[16];
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned int> springCount;
      utils::GPUBasedReference<gpu::dtos::SoftBody> actorIndex;
      explicit Actor() = default;
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange,
        physics::Actor** actors,
        physics::softbody::Actor** subEntities
      );
      void beforeWrite() override;
      void addSpring(utils::AllocationRange* springAllocationRange, unsigned char nodeIndex);
    private:
      utils::ResourceAllocation<gpu::dtos::SoftBody> subAllocation;
      utils::ResourceAllocation<physics::softbody::Actor*> subEntityAllocation;
      //physics::softbody::Actor** subEntities = nullptr;
      //utils::EventHandler<utils::AllocationMoveEvent> moveSubEventHandler = {};
      //utils::EventHandler<utils::DeallocationEvent> subDeallocationEventHandler = {};
  };
}
#define INIT_GPU_BASED_SOFTBODY_PROPERTY(typeName, propName, value) { INIT_GPU_BASED_PROPERTY(typeName, subAllocation, propName); (propName).set(value);}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
