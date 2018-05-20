#include "Simulator.h"


namespace alcube::physics {
  using namespace utils::opencl::kernelargs;

  Simulator::Simulator(
    unsigned int maxActorCount,
    Grid* grid,
    float deltaTime,
    float gravity,
    gpu::GPUAccessor* gpuAccessor,
    ActorResources* actorResources
  ) {
    this->gpuAccessor = gpuAccessor;
    this->actorResources = actorResources;
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->deltaTime = deltaTime;
    this->maxActorCount = maxActorCount;
    this->grid = grid;
    this->allGridCount = grid->xCount * grid->yCount * grid->zCount;
    this->gravity = gravity;
    sphericalShellRadius = 100000.0f;
    activeActorCount = 0;
    allActorCount = 0;
  }

  unsigned short Simulator::getActorCount() {
    return (unsigned short)activeActorCount;
  }

  void Simulator::setUpConstants() {
    memories.grid.dto[0] = grid->dto;
    for (auto subSimulator : subSimulators) {
      subSimulator->setUpConstants();
    }
    kernels.inputPhysicsConstants(
      1,
      memories.constants,
      memories.grid,
      gravity,
      deltaTime,
      sphericalShellRadius
    );
  }

  void Simulator::input() {
    allActorCount = (unsigned short)actorResources->allocationRange->getAllocatedLength();
    auto updateCount = allActorCount - activeActorCount;
    if (updateCount == 0) {
      return;
    }

    for (auto i = activeActorCount; i < allActorCount; i++) {
      actorResources->entities[i]->beforeWrite();
    }
    updateGPUResourcesCount();
    memories.hostActors.write(activeActorCount);
    kernels.inputActors(
      updateCount,
      memories.hostActors,
      memories.actorStates,
      (unsigned short)activeActorCount
    );
    activeActorCount = allActorCount;

    for (auto subSimulator: subSimulators) {
      subSimulator->input();
    }
  }

  void Simulator::updateGPUResourcesCount() {
    unsigned int actorCountForBitonicSort = utils::math::powerOf2(allActorCount);
    memories.hostActors.setCount(allActorCount);
    memories.actorStates.setCount(allActorCount);
    memories.gridAndActorRelations.setCount(actorCountForBitonicSort);
  }

  void Simulator::computeBroadPhase() {
    if (activeActorCount < 2) {
      return;
    }
    unsigned int actorCountForBitonicSort = utils::math::powerOf2(activeActorCount);
    auto maxActorCountShort = (unsigned short)maxActorCount;
    // Initialize grid and actor relations
    kernels.initGridAndActorRelations(
      actorCountForBitonicSort,
      memories.gridAndActorRelations,
      allGridCount,
      maxActorCountShort
    );

    // Set grid index to rigid body state, and register grid and actor relations.
    kernels.fillGridIndex(
      activeActorCount,
      memories.constants,
      memories.actorStates,
      memories.gridAndActorRelations
    );

    // Sort grid and actor relations (Bitonic sort)
    auto stageCount = (int)log2(actorCountForBitonicSort);
    int passCount = 0;
    for (unsigned int i = 0; i < stageCount - 1; i++) {
      for (unsigned int j = 0; j < passCount + 1; j++) {
        auto distance = 1u << (i - j);
        auto stageDistance = 1u << i;
        kernels.bitonic(
          actorCountForBitonicSort,
          memories.gridAndActorRelations,
          distance,
          stageDistance
        );
      }
      passCount++;
    }
    passCount = stageCount;
    for (unsigned int i = 0; i < passCount; i++) {
      auto distance = 1u << (stageCount - (i + 1));
      kernels.merge(
        actorCountForBitonicSort,
        memories.gridAndActorRelations,
        distance
      );
    }

    // Setup grid and actor relation ranges
    memories.gridStartIndices.zeroFill();
    memories.gridEndIndices.zeroFill();
    kernels.setGridRelationIndexRange(
      activeActorCount > 1 ? activeActorCount - 1 : 1,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      activeActorCount
    );
  }

  void Simulator::computeNarrowPhase() {
    kernels.collectIntersections(
      activeActorCount,
      memories.actorStates,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      memories.constants
    );
  }

  void Simulator::updateForce() {
    kernels.initStepVariables(
      activeActorCount,
      memories.actorStates,
      memories.constants
    );

    for (auto subSimulator : subSimulators) {
      subSimulator->updateForce();
    }
  }

  void Simulator::motion() {
    for (auto subSimulator : subSimulators) {
      subSimulator->motion();
    }

    kernels.postProcessing(
      activeActorCount,
      memories.constants,
      memories.actorStates
    );
  }

  void Simulator::setUp() {
    setUpConstants();
    clFinish(gpuAccessor->resourcesProvider->queue->queue);
  }

  void Simulator::update() {
    input();
    computeBroadPhase();
    computeNarrowPhase();
    updateForce();
    motion();
  }

  void Simulator::add(SubSimulator *subSimulator) {
    subSimulators.push_back(subSimulator);
  }

  void Simulator::gc() {
    memories.actors.setCount(activeActorCount);
    kernels.outputActors(activeActorCount, memories.actors, memories.actorStates);
    memories.actors.read(&memories.hostActors);
    memories.softBodies.read(&memories.hostSoftBodies);
    memories.fluids.read(&memories.hostFluids);
    memories.springs.read(&memories.springs);
    actorResources->allocationRange->gc();
    actorResources->softbodyResource->allocationRange->gc();
    actorResources->fluidResource->allocationRange->gc();
    actorResources->springResource->allocationRange->gc();
    activeActorCount = 0;
    input();
  }
}
