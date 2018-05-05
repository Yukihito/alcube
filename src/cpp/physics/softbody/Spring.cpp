#include "Spring.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  void SpringNode::init(
    utils::AllocationRange* allocationRange,
    alcube::utils::ResourceAllocation<alcube::gpu::dtos::Spring> *spring,
    unsigned char nodeIndex
  ) {
    this->allocationRange = allocationRange;
    this->spring = spring;
    this->nodeIndex = nodeIndex;
  }

  void SpringNode::setPosition(glm::vec3 position) {
    spring->getPtr()->nodePositionsModelSpace[nodeIndex] = toCl(position);
  }

  void SpringNode::setActor(alcube::physics::softbody::Actor *actor) {
    this->actor = actor;
    spring->getPtr()->actorIndices[nodeIndex] = actor->getIndex();
    auto subPhysicalQuantity = actor->getSubPhysicalQuantity();
    subPhysicalQuantity->springIndices[subPhysicalQuantity->springCount] = allocationRange->getIndex();
    subPhysicalQuantity->springNodeIndices[subPhysicalQuantity->springCount] = nodeIndex;
    subPhysicalQuantity->springCount++;
  }

  void Spring::setK(float k) {
    spring.getPtr()->k = k;
  }

  void Spring::init(gpu::GPUAccessor *gpuAccessor, utils::AllocationRange *allocationRange) {
    spring.init(allocationRange, gpuAccessor->dtos.springs);
    this->allocationRange = allocationRange;
    for (unsigned char i = 0; i < 2; i++) {
      nodes[i].init(allocationRange, &spring, i);
    }
  }

  SpringNode* Spring::getNode(unsigned int index) {
    return &nodes[index];
  }

  unsigned int Spring::getIndex() {
    return allocationRange->getIndex();
  }
}
