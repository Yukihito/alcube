#include <gtest/gtest.h>
#include "../../../src/cpp/utils/FileUtil.h"
#include "../../../src/cpp/utils/opencl/Resources.h"
#include "../../../src/cpp/physics/Simulator.h"

class SimulatorTest : public ::testing::Test { // NOLINT
  protected:
    alcube::physics::Simulator* simulator;
    alcube::utils::opencl::Resources* resources;
    std::mutex mutex;
    virtual void SetUp() { // NOLINT
      unsigned int maxCellCount = 16384; // 2^14
      //unsigned int maxCellCount = 64;

      float deltaTime = 1000.0f / 30.0f;
      resources = new alcube::utils::opencl::Resources();
      auto fileUtil = new alcube::utils::FileUtil();
      simulator = new alcube::physics::Simulator(
        resources,
        fileUtil,
        &mutex,
        maxCellCount,
        8,
        64,
        64,
        64
      );
      std::cout << "up" << std::endl;
    }

    virtual void TearDown() { // NOLINT
      resources->release();
      std::cout << "down" << std::endl;
    }
};

TEST_F(SimulatorTest, test1) { // NOLINT
  ASSERT_EQ(1 < 2, true);
}