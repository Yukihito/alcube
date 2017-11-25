#include "BenchmarkSample.h"

namespace alcube::physics::samples {
  void BenchmarkSample::runApp(int argc, char **argv) {
    unsigned int maxCellCount = 16384; // 2^14
    std::mutex mutex;
    float deltaTime = 1.0f / 30.0f;
    auto resources = new utils::opencl::Resources();
    auto fileUtil = new utils::FileUtil();
    auto simulator = new Simulator(
      resources,
      fileUtil,
      &mutex,
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
      auto cell = new Cell();
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
      simulator->dtos.currentStates[i].gridIndex;
    }

    for (int i = 0; i < 1000; i++) {
      profiler->start(timerId);
      simulator->update(deltaTime);
      profiler->stop(timerId);
    }

    resources->release();
    profiler->update();
  }
}