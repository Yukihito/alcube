#include <gtest/gtest.h>
#include <random>
#include "../../../src/cpp/utils/FileUtil.h"
#include "../../../src/cpp/utils/opencl/Resources.h"
#include "../../../src/cpp/physics/Simulator.h"

namespace alcube::physics {
  class SimulatorTest : public ::testing::Test { // NOLINT
    protected:
      Simulator *simulator;
      utils::opencl::Resources *resources;
      std::mutex mutex;
      std::vector<alcube::physics::Cell *> cells;
      unsigned short maxCellCount = 16384; // 2^14
      unsigned int gridEdgeLength = 8;
      unsigned int xGridCount = 64;
      unsigned int yGridCount = 64;
      unsigned int zGridCount = 64;
      unsigned int allGridCount = 64 * 64 * 64;

      virtual void SetUp() { // NOLINT
        float deltaTime = 1000.0f / 30.0f;
        resources = new utils::opencl::Resources();
        auto fileUtil = new utils::FileUtil();
        simulator = new Simulator(
          resources,
          fileUtil,
          &mutex,
          maxCellCount,
          gridEdgeLength,
          xGridCount,
          yGridCount,
          zGridCount
        );
      }

      virtual void TearDown() { // NOLINT
        resources->release();
        for (Cell *cell: cells) {
          delete cell;
        }
        cells.clear();
      }

      void createCells(int count) {
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_real_distribution<float> randReal(-50, 50);

        for (int i = 0; i < count; i++) {
          auto cell = new Cell();
          cell->currentState.position = glm::vec3(
            randReal(mt),
            randReal(mt),
            randReal(mt)
          );
          addCell(cell);
        }
      }

      void addCell(Cell* cell) {
        simulator->cells.push_back(cell);
        cells.push_back(cell);
      }

      void simulateBroadPhase() {
        simulator->setUpComputingSize();
        simulator->input();
        simulator->setUpMemories();
        simulator->computeBroadPhase();
        simulator->read(simulator->memories.gridAndCellRelations, simulator->dtos.gridAndCellRelations);
        simulator->read(simulator->memories.gridStartIndices, simulator->dtos.gridStartIndices);
        simulator->read(simulator->memories.gridEndIndices, simulator->dtos.gridEndIndices);
        simulator->tearDownMemories();
      }

      void simulateNarrowPhase(float deltaTime) {
        simulator->setUpComputingSize();
        simulator->input();
        simulator->setUpMemories();
        simulator->computeBroadPhase();
        simulator->computeNarrowPhase(deltaTime);
        simulator->read(simulator->memories.cells, simulator->dtos.cells);
        simulator->tearDownMemories();
      }
  };

  TEST_F(SimulatorTest, broadPhase1) { // NOLINT
    unsigned short allCellCount = maxCellCount;
    createCells(allCellCount);
    simulateBroadPhase();

    bool foundInvalidOrderRelation = false;
    bool foundInvalidGridIndexRelation = false;
    bool foundInvalidCellIndexRelation = false;
    unsigned int prevGridIndex = simulator->dtos.gridAndCellRelations[0].gridIndex;
    for (int i = 0; i < allCellCount; i++) {
      auto relation = simulator->dtos.gridAndCellRelations[i];
      if (relation.gridIndex < prevGridIndex) {
        foundInvalidOrderRelation = true;
        break;
      }
      if (relation.gridIndex >= allGridCount) {
        foundInvalidGridIndexRelation = true;
        break;
      }
      if (relation.cellIndex >= allCellCount) {
        foundInvalidCellIndexRelation = true;
        break;
      }
      prevGridIndex = relation.gridIndex;
    }
    ASSERT_FALSE(foundInvalidOrderRelation);
    ASSERT_FALSE(foundInvalidCellIndexRelation);
    ASSERT_FALSE(foundInvalidGridIndexRelation);

    unsigned short registeredCellCount = 0;
    bool foundInvalidCellIndex = false;
    bool foundInvalidGridIndex = false;
    for (unsigned int i = 0; i < allGridCount; i++) {
      for (unsigned int j = simulator->dtos.gridStartIndices[i]; j < simulator->dtos.gridEndIndices[i]; j++) {
        registeredCellCount++;
        auto relation = simulator->dtos.gridAndCellRelations[j];
        if (relation.cellIndex >= allCellCount) {
          foundInvalidCellIndex = true;
        }
        if (relation.gridIndex >= allGridCount) {
          foundInvalidGridIndex = true;
        }
      }
    }
    ASSERT_FALSE(foundInvalidCellIndex);
    ASSERT_FALSE(foundInvalidGridIndex);
    ASSERT_EQ(registeredCellCount, allCellCount);
  }

  TEST_F(SimulatorTest, broadPhase2) { // NOLINT
    // Expected gridAndCellRelations are
    // 0 ~ allCellCount - 1               : Sorted relations.
    // allCellCount ~ sortTargetCount - 1 : Padding values.
    // sortTargetCount ~ maxCellCount     : Not initialized values.
    unsigned short paddingCount = 10;
    auto sortTargetCount = (unsigned short)(maxCellCount / 2);
    unsigned short allCellCount = sortTargetCount - paddingCount;
    createCells(allCellCount);
    simulateBroadPhase();

    bool foundInvalidOrderRelation = false;
    bool foundInvalidGridIndexRelation = false;
    bool foundInvalidCellIndexRelation = false;
    unsigned int prevGridIndex = simulator->dtos.gridAndCellRelations[0].gridIndex;
    for (int i = 0; i < allCellCount; i++) {
      auto relation = simulator->dtos.gridAndCellRelations[i];
      if (relation.gridIndex < prevGridIndex) {
        foundInvalidOrderRelation = true;
        break;
      }
      if (relation.gridIndex >= allGridCount) {
        foundInvalidGridIndexRelation = true;
        break;
      }
      if (relation.cellIndex >= allCellCount) {
        foundInvalidCellIndexRelation = true;
        break;
      }
      prevGridIndex = relation.gridIndex;
    }
    ASSERT_FALSE(foundInvalidOrderRelation);
    ASSERT_FALSE(foundInvalidCellIndexRelation);
    ASSERT_FALSE(foundInvalidGridIndexRelation);
    bool foundInvalidPaddingRelation = false;
    for (int i = allCellCount; i < sortTargetCount; i++) {
      auto relation = simulator->dtos.gridAndCellRelations[i];
      if (relation.gridIndex != allGridCount || relation.cellIndex != maxCellCount) {
        foundInvalidPaddingRelation = true;
      }
    }
    ASSERT_FALSE(foundInvalidPaddingRelation);

    unsigned short registeredCellCount = 0;
    bool foundInvalidCellIndex = false;
    bool foundInvalidGridIndex = false;
    for (unsigned int i = 0; i < allGridCount; i++) {
      for (unsigned int j = simulator->dtos.gridStartIndices[i]; j < simulator->dtos.gridEndIndices[i]; j++) {
        registeredCellCount++;
        auto relation = simulator->dtos.gridAndCellRelations[j];
        if (relation.cellIndex >= allCellCount) {
          foundInvalidCellIndex = true;
        }
        if (relation.gridIndex >= allGridCount) {
          foundInvalidGridIndex = true;
        }
      }
    }
    ASSERT_FALSE(foundInvalidCellIndex);
    ASSERT_FALSE(foundInvalidGridIndex);
    ASSERT_EQ(registeredCellCount, allCellCount);
  }

  TEST_F(SimulatorTest, narrowPhase1) { // NOLINT
    // Doesn't move each other, and already intersected.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    Cell* cell0 = new Cell();
    cell0->currentState.position = glm::vec3(0.0f, 0.0f, 0.0f);

    Cell* cell1 = new Cell();
    cell1->currentState.position = glm::vec3(2.0f - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.cells[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[0].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.cells[0].neighborCellIndices[0], 1);

    ASSERT_FALSE(simulator->dtos.cells[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[1].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.cells[1].neighborCellIndices[0], 0);
  }

  TEST_F(SimulatorTest, narrowPhase2) { // NOLINT
    // Doesn't move and intersect.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    Cell* cell0 = new Cell();
    cell0->currentState.position = glm::vec3(0.0f, 0.0f, 0.0f);

    Cell* cell1 = new Cell();
    cell1->currentState.position = glm::vec3(2.0f + smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.cells[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[0].neighborCellCount, 0);

    ASSERT_FALSE(simulator->dtos.cells[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[1].neighborCellCount, 0);
  }

  TEST_F(SimulatorTest, narrowPhase3) { // NOLINT
    // Cell0 approaches cell1, but does not intersect during delta time.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    Cell* cell0 = new Cell();
    cell0->currentState.position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->currentState.linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->currentState.linearMomentum.x / cell0->mass) * deltaTime;

    Cell* cell1 = new Cell();
    cell1->currentState.position = glm::vec3(2.0f + movingDistance + smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.cells[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[0].neighborCellCount, 0);

    ASSERT_FALSE(simulator->dtos.cells[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[1].neighborCellCount, 0);
  }

  TEST_F(SimulatorTest, narrowPhase4) { // NOLINT
    // Cell0 approaches cell1, and intersects during delta time.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    Cell* cell0 = new Cell();
    cell0->currentState.position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->currentState.linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->currentState.linearMomentum.x / cell0->mass) * deltaTime;

    Cell* cell1 = new Cell();
    cell1->currentState.position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_TRUE(simulator->dtos.cells[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[0].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.cells[0].neighborCellIndices[0], 1);
    ASSERT_EQ(simulator->dtos.cells[0].collisionCellIndex, 1);

    ASSERT_TRUE(simulator->dtos.cells[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.cells[1].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.cells[1].neighborCellIndices[0], 0);
    ASSERT_EQ(simulator->dtos.cells[1].collisionCellIndex, 0);
  }
}