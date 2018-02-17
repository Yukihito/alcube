#include <gtest/gtest.h>
#include <random>
#include "../../../src/cpp/utils/FileUtil.h"
#include "../../../src/cpp/utils/opencl/Resources.h"
#include "../../../src/cpp/physics/Simulator.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
namespace alcube::physics {
  class SimulatorTest : public ::testing::Test { // NOLINT
    protected:
      Simulator *simulator;
      utils::opencl::Resources *resources;
      std::mutex mutex;
      std::vector<alcube::physics::SoftBodyParticle *> cells;
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
        for (SoftBodyParticle *cell: cells) {
          delete cell;
        }
        cells.clear();
      }

      void createCells(int count) {
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_real_distribution<float> randReal(-50, 50);

        for (int i = 0; i < count; i++) {
          auto cell = new SoftBodyParticle();
          cell->position = glm::vec3(
            randReal(mt),
            randReal(mt),
            randReal(mt)
          );
          addCell(cell);
        }
      }

      void addCell(SoftBodyParticle* cell) {
        simulator->add(cell);
        cells.push_back(cell);
      }

      void simulateBroadPhase() {
        simulator->setUpComputingSize();
        simulator->input();
        simulator->setUpMemories();
        simulator->computeBroadPhase();
        simulator->read(simulator->memories._gridAndActorRelations, simulator->dtos.gridAndActorRelations);
        simulator->read(simulator->memories._gridStartIndices, simulator->dtos.gridStartIndices);
        simulator->read(simulator->memories._gridEndIndices, simulator->dtos.gridEndIndices);
        simulator->tearDownMemories();
      }

      void simulateNarrowPhase(float deltaTime) {
        simulator->setUpComputingSize();
        simulator->input();
        simulator->setUpMemories();
        simulator->computeBroadPhase();
        simulator->computeNarrowPhase(deltaTime);
        simulator->read(simulator->memories._actorStates, simulator->dtos.actorStates);
        simulator->tearDownMemories();
      }

      void simulateAll(float deltaTime) {
        simulator->setUpComputingSize();
        simulator->input();
        simulator->setUpMemories();
        simulator->computeBroadPhase();
        simulator->computeNarrowPhase(deltaTime);
        simulator->motion(deltaTime);
        simulator->resolveIntersection();
        simulator->read(simulator->memories._actorStates, simulator->dtos.actorStates);
        simulator->read(simulator->memories._nextStates, simulator->dtos.nextStates);
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
    unsigned int prevGridIndex = simulator->dtos.gridAndActorRelations[0].gridIndex;
    for (int i = 0; i < allCellCount; i++) {
      auto relation = simulator->dtos.gridAndActorRelations[i];
      if (relation.gridIndex < prevGridIndex) {
        foundInvalidOrderRelation = true;
        break;
      }
      if (relation.gridIndex >= allGridCount) {
        foundInvalidGridIndexRelation = true;
        break;
      }
      if (relation.actorIndex >= allCellCount) {
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
        auto relation = simulator->dtos.gridAndActorRelations[j];
        if (relation.actorIndex >= allCellCount) {
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
    // Expected gridAndActorRelations are
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
    unsigned int prevGridIndex = simulator->dtos.gridAndActorRelations[0].gridIndex;
    for (int i = 0; i < allCellCount; i++) {
      auto relation = simulator->dtos.gridAndActorRelations[i];
      if (relation.gridIndex < prevGridIndex) {
        foundInvalidOrderRelation = true;
        break;
      }
      if (relation.gridIndex >= allGridCount) {
        foundInvalidGridIndexRelation = true;
        break;
      }
      if (relation.actorIndex >= allCellCount) {
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
      auto relation = simulator->dtos.gridAndActorRelations[i];
      if (relation.gridIndex != allGridCount || relation.actorIndex != maxCellCount) {
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
        auto relation = simulator->dtos.gridAndActorRelations[j];
        if (relation.actorIndex >= allCellCount) {
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

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellIndices[0], 1);

    ASSERT_FALSE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellIndices[0], 0);
  }

  TEST_F(SimulatorTest, narrowPhase2) { // NOLINT
    // Doesn't move and intersect.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 0);

    ASSERT_FALSE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 0);
  }

  TEST_F(SimulatorTest, narrowPhase3) { // NOLINT
    // Cell0 approaches cell1, but does not intersect during delta time.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance + smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 0);

    ASSERT_FALSE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 0);
  }

  TEST_F(SimulatorTest, narrowPhase4) { // NOLINT
    // Cell0 approaches cell1, and intersects during delta time.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_TRUE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellIndices[0], 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].collisionCellIndex, 1);

    ASSERT_TRUE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellIndices[0], 0);
    ASSERT_EQ(simulator->dtos.actorStates[1].collisionCellIndex, 0);
  }

  TEST_F(SimulatorTest, narrowPhase5) { // NOLINT
    // Cell0 intersects cell1, but steps away during delta time.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(-1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance + smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 0);

    ASSERT_FALSE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 0);
  }

  TEST_F(SimulatorTest, narrowPhase6) { // NOLINT
    // Cell0 keeps an intersection with cell1 during delta time.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(-1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateNarrowPhase(deltaTime);

    ASSERT_FALSE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellIndices[0], 1);

    ASSERT_FALSE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellIndices[0], 0);
  }

  TEST_F(SimulatorTest, narrowPhase7) { // NOLINT
    // Cell0 intersects cell1 and cell2.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.5f, 0.0f);

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + smallDistance, 0.0f, 0.0f);
    auto cell2 = new SoftBodyParticle();
    cell1->position = glm::vec3(0.0f, 2.0f + smallDistance, 0.0f);

    addCell(cell0);
    addCell(cell1);
    addCell(cell2);
    simulateNarrowPhase(deltaTime);

    ASSERT_TRUE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].collisionCellIndex, 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellCount, 2);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellIndices[0], 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].neighborCellIndices[1], 2);

    ASSERT_TRUE(simulator->dtos.actorStates[1].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[1].collisionCellIndex, 0);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[1].neighborCellIndices[0], 0);

    ASSERT_FALSE(simulator->dtos.actorStates[2].collisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[2].neighborCellCount, 1);
    ASSERT_EQ(simulator->dtos.actorStates[2].neighborCellIndices[0], 0);
  }

  TEST_F(SimulatorTest, all1) { // NOLINT
    // Elastic collision.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateAll(deltaTime);

    ASSERT_EQ(simulator->dtos.nextStates[0].linearMomentum.x, 0.0f);
    ASSERT_EQ(simulator->dtos.nextStates[1].linearMomentum.x, 1.0f);
  }

  TEST_F(SimulatorTest, all2) { // NOLINT
    // Inelastic collision.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    cell0->elasticity = 0.5f;
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateAll(deltaTime);

    ASSERT_EQ(simulator->dtos.nextStates[0].linearMomentum.x, 0.25f);
    ASSERT_EQ(simulator->dtos.nextStates[1].linearMomentum.x, 0.75f);
  }

  TEST_F(SimulatorTest, all3) { // NOLINT
    // Perfectly inelastic collision.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.0f, 0.0f);
    cell0->elasticity = 0.0f;
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);

    addCell(cell0);
    addCell(cell1);
    simulateAll(deltaTime);

    ASSERT_EQ(simulator->dtos.nextStates[0].linearMomentum.x, 0.5f);
    ASSERT_EQ(simulator->dtos.nextStates[1].linearMomentum.x, 0.5f);
  }

  TEST_F(SimulatorTest, all4) { // NOLINT
    // Collision with corner (Grid start position) occurs.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;
    glm::vec3 corner = -glm::vec3(xGridCount * gridEdgeLength, yGridCount * gridEdgeLength, zGridCount * gridEdgeLength) / 2.0f;
    glm::vec3 smallMove = glm::vec3(smallDistance, smallDistance, smallDistance);
    auto cell = new SoftBodyParticle();
    cell->position = glm::vec3(cell->radius, cell->radius, cell->radius) + smallMove + corner;
    cell->linearMomentum = glm::vec3(-0.5f, -1.0f, -0.5f);
    addCell(cell);
    simulateAll(deltaTime);
    ASSERT_TRUE(simulator->dtos.actorStates[0].wallCollisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].collisionWallAxis, 1);
    ASSERT_GT(simulator->dtos.nextStates[0].linearMomentum.x, -0.5f);
    ASSERT_GT(simulator->dtos.nextStates[0].linearMomentum.y, 1.0f);
    ASSERT_GT(simulator->dtos.nextStates[0].linearMomentum.z, -0.5f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.x, corner.x + 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.y, corner.y + 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.z, corner.z + 1.0f);
  }

  TEST_F(SimulatorTest, all5) { // NOLINT
    // SoftBodyParticle intersects corner (Grid start position) occurs.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;
    glm::vec3 corner = -glm::vec3(xGridCount * gridEdgeLength, yGridCount * gridEdgeLength, zGridCount * gridEdgeLength) / 2.0f;
    glm::vec3 smallMove = glm::vec3(smallDistance, smallDistance, smallDistance);
    auto cell = new SoftBodyParticle();
    cell->position = glm::vec3(cell->radius, cell->radius, cell->radius) - smallMove + corner;
    addCell(cell);
    simulateAll(deltaTime);
    ASSERT_FALSE(simulator->dtos.actorStates[0].wallCollisionOccurred);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.x, corner.x + 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.y, corner.y + 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.z, corner.z + 1.0f);
  }

  TEST_F(SimulatorTest, all6) { // NOLINT
    // Collision with corner (Grid end position) occurs.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;
    glm::vec3 corner = glm::vec3(xGridCount * gridEdgeLength, yGridCount * gridEdgeLength, zGridCount * gridEdgeLength) / 2.0f;
    glm::vec3 smallMove = glm::vec3(smallDistance, smallDistance, smallDistance);
    auto cell = new SoftBodyParticle();
    cell->position = -glm::vec3(cell->radius, cell->radius, cell->radius) - smallMove + corner;
    cell->linearMomentum = glm::vec3(0.5f, 1.0f, 0.5f);
    addCell(cell);
    simulateAll(deltaTime);
    ASSERT_TRUE(simulator->dtos.actorStates[0].wallCollisionOccurred);
    ASSERT_EQ(simulator->dtos.actorStates[0].collisionWallAxis, 1);
    ASSERT_EQ(simulator->dtos.actorStates[0].collisionType, 0);
    ASSERT_LT(simulator->dtos.nextStates[0].linearMomentum.x, 0.5f);
    ASSERT_LT(simulator->dtos.nextStates[0].linearMomentum.y, -1.0f);
    ASSERT_LT(simulator->dtos.nextStates[0].linearMomentum.z, 0.5f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.x, corner.x - 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.y, corner.y - 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.z, corner.z - 1.0f);
  }

  TEST_F(SimulatorTest, all7) { // NOLINT
    // SoftBodyParticle intersects corner (Grid end position) occurs.
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;
    glm::vec3 corner = glm::vec3(xGridCount * gridEdgeLength, yGridCount * gridEdgeLength, zGridCount * gridEdgeLength) / 2.0f;
    glm::vec3 smallMove = glm::vec3(smallDistance, smallDistance, smallDistance);
    auto cell = new SoftBodyParticle();
    cell->position = -glm::vec3(cell->radius, cell->radius, cell->radius) + smallMove + corner;
    addCell(cell);
    simulateAll(deltaTime);
    ASSERT_FALSE(simulator->dtos.actorStates[0].wallCollisionOccurred);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.x, corner.x - 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.y, corner.y - 1.0f);
    ASSERT_EQ(simulator->dtos.nextStates[0].position.z, corner.z - 1.0f);
  }

  TEST_F(SimulatorTest, all8) {
    //  momentum conservation law
    float deltaTime = 1.0f / 30.0f;
    float smallDistance = 0.001f;

    auto cell0 = new SoftBodyParticle();
    cell0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    cell0->linearMomentum = glm::vec3(1.0f, 0.5f, 0.0f);
    float movingDistance = (cell0->linearMomentum.x / cell0->mass) * deltaTime;

    auto cell1 = new SoftBodyParticle();
    cell1->position = glm::vec3(2.0f + movingDistance - smallDistance, 0.0f, 0.0f);
    cell1->linearMomentum = glm::vec3(-1.0f, -0.5f, 0.0f);

    float momentumSum = sqrtf(1.0f * 1.0f + 0.5f * 0.5f) * 2.0f;

    addCell(cell0);
    addCell(cell1);
    simulateAll(deltaTime);
    ASSERT_TRUE(simulator->dtos.actorStates[0].collisionOccurred);
    ASSERT_TRUE(simulator->dtos.actorStates[1].collisionOccurred);
    cl_float3 m0 = simulator->dtos.nextStates[0].linearMomentum;
    cl_float3 m1 = simulator->dtos.nextStates[1].linearMomentum;

    float momentumSum2 = sqrtf(m0.x * m0.x + m0.y * m0.y + m0.z * m0.z) + sqrtf(m1.x * m1.x + m1.y * m1.y + m1.z * m1.z);

    ASSERT_EQ(momentumSum, momentumSum2);
  }
}
#pragma clang diagnostic pop