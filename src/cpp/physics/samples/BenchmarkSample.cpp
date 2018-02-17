#include "BenchmarkSample.h"

namespace alcube::physics::samples {
  void BenchmarkSample::runApp(int argc, char **argv) {
    unsigned int maxCellCount = 16384; // 2^14
    float deltaTime = 1.0f / 30.0f;
    auto fileUtil = new utils::FileUtil();
    auto resourcesProvider = new utils::opencl::ResourcesProvider(fileUtil, new utils::opencl::Resources());
    auto simulator = new Simulator(
      resourcesProvider,
      maxCellCount,
      8,
      64,
      64,
      64
    );
    auto profiler = new utils::Profiler();
    profiler->enabled = true;
    profiler->setShowInterval(0);
    int timerId = profiler->create("update");
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(-50, 50);
    std::uniform_real_distribution<float> randReal2(-1, 1);

    for (int i = 0; i < maxCellCount; i++) {
      auto cell = new SoftBodyParticle();
      cell->position = glm::vec3(
        randReal(mt),
        randReal(mt),
        randReal(mt)
      );
      cell->linearMomentum = glm::vec3(
        randReal2(mt),
        randReal2(mt),
        randReal2(mt)
      );
      simulator->add(cell);
    }

    for (int i = 0; i < 1000; i++) {
      profiler->start(timerId);
      simulator->update(deltaTime);
      profiler->stop(timerId);
    }

    resourcesProvider->resources->release();
    profiler->update();
  }
}