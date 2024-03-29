#include "Spring.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  void SpringNode::init(
    ActorPair* actorPair,
    utils::GPUBasedProperty<gpu::dtos::Spring, cl_float3*>* nodePositionsModelSpace,
    unsigned char nodeIndex
  ) {
    this->actorPair = actorPair;
    this->nodePositionsModelSpace = nodePositionsModelSpace;
    this->nodeIndex = nodeIndex;
  }

  void SpringNode::setPosition(glm::vec3 position) {
    nodePositionsModelSpace->get()[nodeIndex] = toCl(position);
  }

  void SpringNode::setActor(alcube::physics::softbody::Actor *actor) {
    actorPair->setActor(actor, nodeIndex);
  }

  SpringNode* Spring::getNode(unsigned int index) {
    return &nodes[index];
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
    this->entityAllocation.init(allocationRange, entities);
    this->entityAllocation.set(this);
    INIT_GPU_BASED_PROPERTY(float, allocation, k);
    INIT_GPU_BASED_ARRAY_PROPERTY(cl_float3*, allocation, nodePositionsModelSpace);
    for (unsigned char i = 0; i < 2; i++) {
      nodes[i].init(this, &nodePositionsModelSpace, i);
    }
  }

  void Spring::setActor(alcube::physics::softbody::Actor *actor, unsigned char nodeIndex) {
    actorIndices[nodeIndex].init(
      allocation,
      actor->allocationRange,
      [this, nodeIndex] {
        return this->allocation.getPtr()->actorIndices[nodeIndex];
      },
      [this, nodeIndex] (unsigned int arg) {
        this->allocation.getPtr()->actorIndices[nodeIndex] = (unsigned short)arg;
      }
    );
    actorIndices[nodeIndex].enableLifeSync();
    actor->addSpring(allocationRange, nodeIndex);
  }
}
