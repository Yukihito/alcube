#include "Simulator.h"

namespace alcube::physics::softbody {
  using namespace utils::opencl::conversions;
  Simulator::Simulator(gpu::GPUAccessor *gpuAccessor, ActorResources* actorResources) {
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->actorResources = actorResources;
    actorResources->softbodyResource->allocationRange->onAllocationLengthChanged.subscribe([&]{
      unsigned int actorCount = this->actorResources->softbodyResource->allocationRange->getAllocatedLength();
      memories.hostSoftBodies.setCount(actorCount);
      memories.softBodies.setCount(actorCount);
    });

    actorResources->springResource->allocationRange->onAllocationLengthChanged.subscribe([&]{
      unsigned int springCount = this->actorResources->springResource->allocationRange->getAllocatedLength();
      memories.springs.setCount(springCount);
      memories.springStates.setCount(springCount);
    });
  }

  void Simulator::setUpConstants() {}

  void Simulator::input() {
    unsigned int actorCount = actorResources->softbodyResource->allocationRange->getAllocatedLength();
    unsigned int springCount = actorResources->springResource->allocationRange->getAllocatedLength();
    memories.hostSoftBodies.write();
    memories.springs.write();
    kernels.inputSoftBodies(
      actorCount,
      memories.hostSoftBodies,
      memories.softBodies,
      0
    );

    kernels.inputSprings(
      springCount,
      memories.springs,
      memories.springStates,
      0
    );
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
}
