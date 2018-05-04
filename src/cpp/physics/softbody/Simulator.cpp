#include "Simulator.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  Simulator::Simulator(gpu::GPUAccessor *gpuAccessor, ActorResources* actorResources) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->actorResources = actorResources;
  }

  void Simulator::setUpConstants() {}

  void Simulator::setUpComputingSize() {
    springCount = (unsigned int)springs.size();
    actorCount = (unsigned int)actors.size();
  }

  void Simulator::writeHostMemories() {
    for (unsigned int i = 0; i < springCount; i++) {
      memories.springs.at(i)->k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }
  }

  void Simulator::setUpMemories() {
    memories.hostSoftBodies.setCount(actorCount);
    memories.softBodies.setCount(actorCount);
    memories.springs.setCount(springCount);
    memories.springStates.setCount(springCount);
    memories.hostSoftBodies.write();
    memories.springs.write();
    kernels.inputSoftBodies(
      actorCount,
      memories.hostSoftBodies,
      memories.softBodies
    );

    kernels.inputSprings(
      springCount,
      memories.springs,
      memories.springStates
    );
  }

  void Simulator::updateForce() {
    kernels.updateByPenaltyImpulse(
      actorCount,
      memories.actorStates,
      memories.softBodies,
      memories.constants
    );

    for (int i = 0; i < constraintResolvingIterationCount; i++) {
      kernels.collectCollisions(
        actorCount,
        memories.actorStates,
        memories.softBodies
      );

      kernels.updateByConstraintImpulse(
        actorCount,
        memories.actorStates,
        memories.softBodies
      );
    }

    kernels.updateByFrictionalImpulse(
      actorCount,
      memories.actorStates,
      memories.softBodies
    );
  }

  void Simulator::motion() {
    for (int i = 0; i < motionIterationCount; i++) {
      if (springCount > 0) {
        kernels.calcSpringImpulses(
          springCount,
          memories.constants,
          memories.springStates,
          memories.physicalQuantities
        );
      }
      kernels.updateBySpringImpulse(
        actorCount,
        memories.constants,
        memories.softBodies,
        memories.actorStates,
        memories.physicalQuantities,
        memories.springStates
      );
    }
  }

  bool Simulator::add(physics::Actor *actor) {
    if (Actor::instances.count(actor) > 0) {
      actors.push_back(Actor::instances[actor]);
      return true;
    } else {
      return false;
    }
  }

  void Simulator::setUpSpring(unsigned int springIndex, unsigned char nodeIndex) {
    unsigned short actorIndex = springs[springIndex]->nodes[nodeIndex].actor->getIndex();
    memories.springs.at(springIndex)->actorIndices[nodeIndex] = actorIndex;
    memories.springs.at(springIndex)->nodePositionsModelSpace[nodeIndex] = toCl(springs[springIndex]->nodes[nodeIndex].position);

    unsigned short softBodyIndex = memories.actors.at(actorIndex)->subPhysicalQuantityIndex;
    auto hostSoftBodyState = memories.hostSoftBodies.at(softBodyIndex);
    hostSoftBodyState->springIndices[hostSoftBodyState->springCount] = springIndex;
    hostSoftBodyState->springNodeIndices[hostSoftBodyState->springCount] = nodeIndex;
    hostSoftBodyState->springCount++;
  }

  void Simulator::add(Spring *spring) {
    springs.push_back(spring);
  }
}
