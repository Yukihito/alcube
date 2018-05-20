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
    INIT_GPU_BASED_REFERENCE_AT(*allocation, actorIndices, actor->allocationRange, nodeIndex);
    auto springCount = actor->springCount.get();
    actor->springIndices.get()[springCount] = allocationRange->getIndex();
    actor->springNodeIndices.get()[springCount] = nodeIndex;
    actor->springCount.set(springCount + 1);
  }

  Spring::Spring() {
    this->moveEventHandler.f = [&](utils::AllocationMoveEvent &e) {
      this->entities[e.dst] = this;
    };

    this->deallocationEventHandler.f = [&](utils::DeallocationEvent &e) {
      this->allocationRange->onMove.unsubscribe(this->moveEventHandler);
      this->allocationRange->onDeallocate.unsubscribe(this->deallocationEventHandler);
    };
  }

  void Spring::setK(float k) {
     this->k.set(k);
  }

  void Spring::init(
    gpu::GPUAccessor *gpuAccessor,
    utils::AllocationRange *allocationRange,
    physics::softbody::Spring** entities
  ) {
    this->allocationRange = allocationRange;
    this->allocation.init(allocationRange, gpuAccessor->dtos.springs);
    this->entities = entities;
    this->entities[allocationRange->getIndex()] = this;
    this->allocationRange->onMove.subscribe(moveEventHandler);
    this->allocationRange->onDeallocate.subscribe(deallocationEventHandler);
    INIT_GPU_BASED_PROPERTY(float, allocation, k);
    for (unsigned char i = 0; i < 2; i++) {
      nodes[i].init(allocationRange, allocation, i);
    }
  }

  SpringNode* Spring::getNode(unsigned int index) {
    return &nodes[index];
  }
}
