#ifndef ALCUBE_SPRING_H
#define ALCUBE_SPRING_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Actor.h"
#include "../../utils/opencl/conversions.h"

namespace alcube::physics::softbody {
  class SpringNode {
    public:
      void init(
        utils::AllocationRange* allocationRange,
        utils::ResourceAllocation<gpu::dtos::Spring>* spring,
        unsigned char nodeIndex
      );
      void setPosition(glm::vec3 position);
      void setActor(physics::softbody::Actor* actor);
    private:
      utils::AllocationRange* allocationRange;
      utils::ResourceAllocation<gpu::dtos::Spring>* spring;
      physics::softbody::Actor* actor;
      unsigned char nodeIndex;
  };

  class Spring {
    public:
      void init(gpu::GPUAccessor* gpuAccessor, utils::AllocationRange* allocationRange);
      void setK(float k);
      SpringNode* getNode(unsigned int index);
    private:
      utils::AllocationRange* allocationRange;
      utils::ResourceAllocation<gpu::dtos::Spring>* spring;
      SpringNode nodes[2];
  };
}

#endif //ALCUBE_SPRING_H
