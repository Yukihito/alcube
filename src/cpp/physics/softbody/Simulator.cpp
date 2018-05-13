#include "Simulator.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  Simulator::Simulator(gpu::GPUAccessor *gpuAccessor, ActorResources* actorResources) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->actorResources = actorResources;
    activeActorCount = 0;
    allActorCount = 0;
    activeSpringCount = 0;
    allSpringCount = 0;
  }

  void Simulator::setUpConstants() {}

  void Simulator::input() {
    inputActors();
    inputSprings();
  }

  void Simulator::inputActors() {
    allActorCount = actorResources->softbodyResource->allocationRange->getAllocatedLength();
    auto updateCount = allActorCount - activeActorCount;
    if (updateCount == 0) {
      return;
    }
    memories.hostSoftBodies.setCount(allActorCount);
    memories.softBodies.setCount(allActorCount);
    memories.hostSoftBodies.write(activeActorCount);
    kernels.inputSoftBodies(
      updateCount,
      memories.hostSoftBodies,
      memories.softBodies,
      (unsigned short)activeActorCount
    );
    activeActorCount = allActorCount;
  }

  void Simulator::inputSprings() {
    allSpringCount = actorResources->springResource->allocationRange->getAllocatedLength();
    auto updateCount = allSpringCount - activeSpringCount;
    if (updateCount == 0) {
      return;
    }
    memories.springs.setCount(allSpringCount);
    memories.springStates.setCount(allSpringCount);
    memories.springs.write(activeSpringCount);
    kernels.inputSprings(
      updateCount,
      memories.springs,
      memories.springStates,
      activeSpringCount
    );
    activeSpringCount = allSpringCount;
  }

  void Simulator::updateForce() {
    unsigned int actorCount = actorResources->softbodyResource->allocationRange->getAllocatedLength();
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
    unsigned int actorCount = actorResources->softbodyResource->allocationRange->getAllocatedLength();
    unsigned int springCount = actorResources->springResource->allocationRange->getAllocatedLength();
    for (int i = 0; i < motionIterationCount; i++) {
      if (springCount > 0) {
        kernels.calcSpringImpulses(
          springCount,
          memories.constants,
          memories.springStates,
          memories.actorStates
        );
      }
      kernels.updateBySpringImpulse(
        actorCount,
        memories.constants,
        memories.softBodies,
        memories.actorStates,
        memories.springStates
      );
    }
  }
}
