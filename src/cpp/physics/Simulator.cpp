#include "Simulator.h"


namespace alcube::physics {
  using namespace utils::opencl::kernelargs;

  Simulator::Simulator(
    unsigned int maxActorCount,
    unsigned int gridEdgeLength,
    unsigned int xGridCount,
    unsigned int yGridCount,
    unsigned int zGridCount,
    float deltaTime,
    gpu::GPUAccessor* gpuAccessor,
    ActorResources* actorResources
  ) {
    this->gpuAccessor = gpuAccessor;
    this->actorResources = actorResources;
    kernels = gpuAccessor->kernels;
    memories = gpuAccessor->memories;
    this->deltaTime = deltaTime;
    this->maxActorCount = maxActorCount;
    this->allGridCount = xGridCount * yGridCount * zGridCount;
    this->gridEdgeLength = gridEdgeLength;
    this->xGridCount = xGridCount;
    this->yGridCount = yGridCount;
    this->zGridCount = zGridCount;
    gravity = 0.0f;
    sphericalShellRadius = 100000.0f;

    actorResources->allocationRange->onAllocationLengthChanged.subscribe([&]{
      unsigned int actorCount = this->actorResources->allocationRange->getAllocatedLength();
      unsigned int actorCountForBitonicSort = utils::math::powerOf2(actorCount);
      memories.actors.setCount(actorCount);
      memories.actorStates.setCount(actorCount);
      memories.hostPhysicalQuantities.setCount(actorCount);
      memories.physicalQuantities.setCount(actorCount);
      memories.gridAndActorRelations.setCount(actorCountForBitonicSort);
    });
  }

  unsigned short Simulator::getActorCount() {
    return (unsigned short)actorResources->allocationRange->getAllocatedLength();
  }

  void Simulator::setUpConstants() {
    auto grid = memories.grid.at(0);
    grid->edgeLength = gridEdgeLength;
    grid->xCount = xGridCount;
    grid->yCount = yGridCount;
    grid->zCount = zGridCount;
    grid->origin = {
      -(xGridCount * gridEdgeLength / 2.0f),
      -(yGridCount * gridEdgeLength / 2.0f),
      -(zGridCount * gridEdgeLength / 2.0f)
    };

    for (int i = 0; i < 3; i++) {
      grid->normals[i] = {0.0f, 0.0f, 0.0f};
      grid->normals[i].s[i] = 1.0f;
    }

    for (int i = 3; i < 6; i++) {
      grid->normals[i] = {0.0f, 0.0f, 0.0f};
      grid->normals[i].s[i - 3] = -1.0f;
    }

    for (auto subSimulator : subSimulators) {
      subSimulator->setUpConstants();
    }
  }

  void Simulator::setUpMemories() {
    for (auto actor: actors) {
      actor->beforeWrite();
    }
    unsigned int actorCount = actorResources->allocationRange->getAllocatedLength();
    memories.actors.write();
    memories.hostPhysicalQuantities.write();

    float splitDeltaTime = deltaTime / /*motionIterationCount*/8;
    kernels.inputConstants(
      1,
      memories.constants,
      memories.grid,
      memories.fluidSettings,
      gravity,
      deltaTime,
      splitDeltaTime,
      sphericalShellRadius
    );

    kernels.inputActors(
      actorCount,
      memories.actors,
      memories.actorStates,
      memories.hostPhysicalQuantities,
      memories.physicalQuantities
    );

    for (auto subSimulator : subSimulators) {
      subSimulator->setUpMemories();
    }
  }

  void Simulator::computeBroadPhase() {
    unsigned int actorCount = actorResources->allocationRange->getAllocatedLength();
    unsigned int actorCountForBitonicSort = utils::math::powerOf2(actorCount);
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
      actorCount,
      memories.constants,
      memories.physicalQuantities,
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
      actorCount > 1 ? actorCount - 1 : 1,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      actorCount
    );
  }

  void Simulator::computeNarrowPhase() {
    unsigned int actorCount = actorResources->allocationRange->getAllocatedLength();
    kernels.collectIntersections(
      actorCount,
      memories.actorStates,
      memories.physicalQuantities,
      memories.gridAndActorRelations,
      memories.gridStartIndices,
      memories.gridEndIndices,
      memories.constants
    );
  }

  void Simulator::updateForce() {
    unsigned int actorCount = actorResources->allocationRange->getAllocatedLength();
    kernels.initStepVariables(
      actorCount,
      memories.actorStates,
      memories.physicalQuantities,
      memories.constants
    );

    for (auto subSimulator : subSimulators) {
      subSimulator->updateForce();
    }
  }

  void Simulator::motion() {
    unsigned int actorCount = actorResources->allocationRange->getAllocatedLength();
    for (auto subSimulator : subSimulators) {
      subSimulator->motion();
    }

    kernels.postProcessing(
      actorCount,
      memories.constants,
      memories.actorStates,
      memories.physicalQuantities
    );
  }

  void Simulator::setUp() {
    setUpConstants();
    setUpMemories();
  }

  void Simulator::update() {
    computeBroadPhase();
    computeNarrowPhase();
    updateForce();
    motion();
  }

  void Simulator::add(SubSimulator *subSimulator) {
    subSimulators.push_back(subSimulator);
  }
}
