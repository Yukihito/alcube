#include "Simulator.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  void Simulator::set(gpu::GPUAccessor *gpuAccessor) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
  }

  void Simulator::setUpConstants() {}

  void Simulator::setUpComputingSize() {
    springCount = (unsigned int)springs.size();
    actorCount = (unsigned int)actors.size();
  }

  void Simulator::writeHostMemories() {
    for (unsigned short i = 0; i < actorCount; i++) {
      auto actor = actors[i];
      actor->actor.subPhysicalQuantityIndex = i;
      actor->subPhysicalQuantity.actorIndex = actor->index;
      memories.actors.dto[actor->index] = actor->actor;
      memories.hostPhysicalQuantities.dto[actor->index] = actor->physicalQuantity;
      memories.hostSoftBodys.dto[i] = actor->subPhysicalQuantity;
    }

    for (unsigned int i = 0; i < springCount; i++) {
      memories.springs.at(i)->k = springs[i]->k;
      setUpSpring(i, 0);
      setUpSpring(i, 1);
    }
  }

  void Simulator::setUpMemories() {
    memories.hostSoftBodys.setCount(actorCount);
    memories.softBodys.setCount(actorCount);
    memories.springs.setCount(springCount);
    memories.springStates.setCount(springCount);
    memories.hostSoftBodys.write();
    memories.springs.write();
    kernels.inputSoftBodys(
      actorCount,
      memories.hostSoftBodys,
      memories.softBodys
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
      memories.softBodys,
      memories.constants
    );

    for (int i = 0; i < constraintResolvingIterationCount; i++) {
      kernels.collectCollisions(
        actorCount,
        memories.actorStates,
        memories.softBodys
      );

      kernels.updateByConstraintImpulse(
        actorCount,
        memories.actorStates,
        memories.softBodys
      );
    }

    kernels.updateByFrictionalImpulse(
      actorCount,
      memories.actorStates,
      memories.softBodys
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
        memories.softBodys,
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
    unsigned short actorIndex = springs[springIndex]->nodes[nodeIndex].actor->index;
    memories.springs.at(springIndex)->actorIndices[nodeIndex] = actorIndex;
    memories.springs.at(springIndex)->nodePositionsModelSpace[nodeIndex] = toCl(springs[springIndex]->nodes[nodeIndex].position);

    unsigned short softBodyIndex = memories.actors.at(actorIndex)->subPhysicalQuantityIndex;
    auto hostSoftBodyState = memories.hostSoftBodys.at(softBodyIndex);
    hostSoftBodyState->springIndices[hostSoftBodyState->springCount] = springIndex;
    hostSoftBodyState->springNodeIndices[hostSoftBodyState->springCount] = nodeIndex;
    hostSoftBodyState->springCount++;
  }

  void Simulator::add(Spring *spring) {
    springs.push_back(spring);
  }
}
