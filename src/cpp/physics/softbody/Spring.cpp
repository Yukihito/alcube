#include "Spring.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  void SpringNode::init(
    utils::AllocationRange* allocationRange,
    utils::ResourceAllocation<gpu::dtos::Spring>& allocation,
    unsigned char nodeIndex
  ) {
    this->allocation = &allocation;
    this->allocationRange = allocationRange;
    this->nodeIndex = nodeIndex;
    INIT_GPU_BASED_ARRAY_PROPERTY(cl_float3*, allocation, nodePositionsModelSpace);
  }

  void SpringNode::setPosition(glm::vec3 position) {
    nodePositionsModelSpace.get()[nodeIndex] = toCl(position);
  }

  void SpringNode::setActor(alcube::physics::softbody::Actor *actor) {
    INIT_GPU_BASED_REFERENCE_AT(unsigned short, *allocation, actorIndices, actor->allocationRange, nodeIndex);
    auto springCount = actor->springCount.get();
    actor->springIndices.get()[springCount] = allocationRange->getIndex();
    actor->springNodeIndices.get()[springCount] = nodeIndex;
    actor->springCount.set(springCount + 1);
  }

  void Spring::setK(float k) {
     this->k.set(k);
  }

  void Spring::init(gpu::GPUAccessor *gpuAccessor, utils::AllocationRange *allocationRange) {
    allocation.init(allocationRange, gpuAccessor->dtos.springs);
    INIT_GPU_BASED_PROPERTY(float, allocation, k);
    this->allocationRange = allocationRange;
    for (unsigned char i = 0; i < 2; i++) {
      nodes[i].init(allocationRange, allocation, i);
    }
  }

  SpringNode* Spring::getNode(unsigned int index) {
    return &nodes[index];
  }

  unsigned int Spring::getIndex() {
    return allocationRange->getIndex();
  }
}
