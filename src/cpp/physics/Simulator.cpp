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
    gpu::GPUAccessor* gpuAccessor
  ) {
    this->gpuAccessor = gpuAccessor;
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
  }

  void Simulator::setUpConstants() {
    auto grid = memories.grid.at(0);
    grid->edgeLength = gridEdgeLength;
    grid->xCount = xGridCount;
    grid->yCount = yGridCount;
    grid->zCount = zGridCount;
    grid->origin = {
      -(float)((xGridCount * gridEdgeLength) / 2),
      -(float)((yGridCount * gridEdgeLength) / 2),
      -(float)((zGridCount * gridEdgeLength) / 2)
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

  void Simulator::setUpComputingSize() {
    actorCount = (unsigned int)actors.size();
    actorCountForBitonicSort = utils::math::powerOf2(actorCount);
    for (auto subSimulator : subSimulators) {
      subSimulator->setUpComputingSize();
    }
  }

  void Simulator::writeHostMemories() {
    for (unsigned short i = 0; i < actorCount; i++) {
      actors[i]->index = i;
    }

    for (auto subSimulator : subSimulators) {
      subSimulator->writeHostMemories();
    }
  }

  void Simulator::setUpMemories() {
    memories.actors.setCount(actorCount);
    memories.hostPhysicalQuantities.setCount(actorCount);

    memories.actorStates.setCount(actorCount);
    memories.physicalQuantities.setCount(actorCount);
    memories.gridAndActorRelations.setCount(actorCountForBitonicSort);

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
    for (int i = 0; i < stageCount - 1; i++) {
      for (int j = 0; j < passCount + 1; j++) {
        auto distance = (unsigned int)(1 << (i - j));
        auto stageDistance = (unsigned int)(1 << i);
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
    for (int i = 0; i < passCount; i++) {
      auto distance = (unsigned int)(1 << (stageCount - (i + 1)));
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

  void Simulator::input() {
    setUpConstants();
    setUpComputingSize();
    writeHostMemories();
    setUpMemories();
  }

  void Simulator::update() {
    computeBroadPhase();
    computeNarrowPhase();
    updateForce();
    motion();
  }

  void Simulator::add(Actor *actor) {
    actors.push_back(actor);
    for (auto subSimulator : subSimulators) {
      if (subSimulator->add(actor)) {
        return;
      }
    }
  }

  void Simulator::add(SubSimulator *subSimulator) {
    subSimulator->set(gpuAccessor);
    subSimulators.push_back(subSimulator);
  }
}
