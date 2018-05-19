#ifndef ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
#define ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H

#include "../Actor.h"
#include <map>

namespace alcube::physics::softbody {
  class Actor : public physics::Actor {
    public:
      utils::GPUBasedProperty<gpu::dtos::SoftBody, float> elasticity = {};
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned int*> springIndices = {};
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned char*> springNodeIndices = {};
      utils::GPUBasedProperty<gpu::dtos::SoftBody, unsigned int> springCount = {};
      utils::GPUBasedReference<gpu::dtos::SoftBody> actorIndex = {};
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        utils::AllocationRange* subAllocationRange,
        physics::Actor** actors,
        physics::softbody::Actor** subActors
      );
      void beforeWrite() override;
    private:
      utils::GPUBasedStruct<gpu::dtos::SoftBody> subStateStruct = {};
  };
}
#define INIT_GPU_BASED_SOFTBODY_PROPERTY(propName, value) { INIT_GPU_BASED_PROPERTY(gpu::dtos::SoftBody, subStateStruct, propName); (propName).set(value);}

#endif //ALCUBE_PHYSICS_SOFT_BODY_PARTICLE_H
