#ifndef ALCUBE_SPRING_H
#define ALCUBE_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Actor.h"
#include "../../utils/opencl/conversions.h"

namespace alcube::physics::softbody {
  class ActorPair {
    public:
      virtual void setActor(physics::softbody::Actor* actor, unsigned char nodeIndex) = 0;
  };

  class SpringNode {
    public:
      void init(
        ActorPair* actorPair,
        utils::GPUBasedProperty<gpu::dtos::Spring, cl_float3*>* nodePositionsModelSpace,
        unsigned char nodeIndex
      );
      void setPosition(glm::vec3 position);
      void setActor(physics::softbody::Actor* actor);
    private:
      ActorPair* actorPair = nullptr;
      utils::GPUBasedProperty<gpu::dtos::Spring, cl_float3*>* nodePositionsModelSpace = nullptr;
      unsigned char nodeIndex = 0;
  };

  class Spring: public ActorPair {
    public:
      explicit Spring();
      void init(
        gpu::GPUAccessor* gpuAccessor,
        utils::AllocationRange* allocationRange,
        physics::softbody::Spring** entities
      );
      void setK(float k);
      SpringNode* getNode(unsigned int index);
      void setActor(physics::softbody::Actor* actor, unsigned char nodeIndex) override;
    private:
      utils::AllocationRange* allocationRange = {};
      utils::ResourceAllocation<gpu::dtos::Spring> allocation;
      utils::GPUBasedProperty<gpu::dtos::Spring, float> k;
      utils::GPUBasedProperty<gpu::dtos::Spring, cl_float3*> nodePositionsModelSpace;
      utils::GPUBasedReference<gpu::dtos::Spring> actorIndices[2];
      SpringNode nodes[2];
      physics::softbody::Spring** entities = nullptr;
      utils::EventHandler<utils::AllocationMoveEvent> moveEventHandler = {};
      utils::EventHandler<utils::DeallocationEvent> deallocationEventHandler = {};
  };
}

#endif //ALCUBE_SPRING_H
