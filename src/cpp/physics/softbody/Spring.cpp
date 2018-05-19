#include "Spring.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  void SpringNode::init(
    utils::AllocationRange* allocationRange,
    utils::GPUBasedStruct<gpu::dtos::Spring>& springStruct,
    unsigned char nodeIndex
  ) {
    this->springStruct = &springStruct;
    this->allocationRange = allocationRange;
    this->nodeIndex = nodeIndex;
    INIT_GPU_BASED_PROPERTY(gpu::dtos::Spring, springStruct, nodePositionsModelSpace);
  }

  void SpringNode::setPosition(glm::vec3 position) {
    nodePositionsModelSpace.getArray()[nodeIndex] = toCl(position);
  }

  void SpringNode::setActor(alcube::physics::softbody::Actor *actor) {
    INIT_GPU_BASED_REFERENCE_AT(gpu::dtos::Spring, unsigned short, *springStruct, actorIndices, actor->allocationRange, nodeIndex);
    auto springCount = actor->springCount.get();
    actor->springIndices.getArray()[springCount] = allocationRange->getIndex();
    actor->springNodeIndices.getArray()[springCount] = nodeIndex;
    actor->springCount.set(springCount + 1);
  }

  void Spring::setK(float k) {
     this->k.set(k);
  }

  void Spring::init(gpu::GPUAccessor *gpuAccessor, utils::AllocationRange *allocationRange) {
    springStruct.init(gpuAccessor->dtos.springs, gpuAccessor->dtos.springs, allocationRange);
    INIT_GPU_BASED_PROPERTY(gpu::dtos::Spring, springStruct, k);
    this->allocationRange = allocationRange;
    for (unsigned char i = 0; i < 2; i++) {
      nodes[i].init(allocationRange, springStruct, i);
    }
  }

  SpringNode* Spring::getNode(unsigned int index) {
    return &nodes[index];
  }

  unsigned int Spring::getIndex() {
    return allocationRange->getIndex();
  }
}
