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
        utils::GPUBasedStruct<gpu::dtos::Spring>& springStruct,
        unsigned char nodeIndex
      );
      void setPosition(glm::vec3 position);
      void setActor(physics::softbody::Actor* actor);
    private:
      utils::AllocationRange* allocationRange = nullptr;
      utils::GPUBasedStruct<gpu::dtos::Spring>* springStruct = nullptr;
      utils::GPUBasedProperty<gpu::dtos::Spring, cl_float3*> nodePositionsModelSpace = {};
      utils::GPUBasedReference<gpu::dtos::Spring> actorIndices = {};
      unsigned char nodeIndex = 0;
  };

  class Spring {
    public:
      void init(gpu::GPUAccessor* gpuAccessor, utils::AllocationRange* allocationRange);
      void setK(float k);
      unsigned int getIndex();
      SpringNode* getNode(unsigned int index);
    private:
      utils::AllocationRange* allocationRange = {};
      utils::GPUBasedStruct<gpu::dtos::Spring> springStruct = {};
      utils::GPUBasedProperty<gpu::dtos::Spring, float> k = {};
      SpringNode nodes[2];
  };
}

#endif //ALCUBE_SPRING_H
