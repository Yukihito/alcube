#include "BenchmarkSample.h"

namespace alcube::physics::samples {
  void BenchmarkSample::runApp(int argc, char **argv) {
    unsigned int maxCellCount = 16384;
    //unsigned int maxCellCount = 64;
    std::mutex mutex;
    float deltaTime = 1000.0f / 30.0f;
    auto resources = new utils::opencl::Resources();
    auto fileUtil = new utils::FileUtil();
    auto simulator = new Simulator(
      resources,
      fileUtil,
      &mutex,
      maxCellCount,
      8,
      16,
      16,
      16
    );
    auto profiler = new utils::Profiler();
    profiler->enabled = true;
    profiler->setShowInterval(0);
    int timerId = profiler->create("update");
    profiler->update();
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<float> randReal(-50, 50);

    for (int i = 0; i < maxCellCount; i++) {
      auto cell = new Cell();
      cell->currentState.position = glm::vec3(
        randReal(mt),
        randReal(mt),
        randReal(mt)
      );
      simulator->cells.push_back(cell);
      simulator->dtos.currentStates[i].gridIndex;
    }

    for (int i = 0; i < 100; i++) {
      profiler->start(timerId);
      simulator->update(deltaTime);
      profiler->stop(timerId);
    }
    resources->release();

    /*
    for (int i = 0; i < maxCellCount; i++) {
      auto state = simulator->dtos.currentStates[i];
      auto relation = simulator->dtos.gridAndCellRelations[i];
      std::cout << "x: " << state.position.x << ", y: " << state.position.y << ", z: " << state.position.z << ", grid: " << state.gridIndex << std::endl;
      std::cout << "grid: " << relation.gridIndex << ", cell: " << relation.cellIndex << std::endl;
    }*/
    profiler->update();
  }
}