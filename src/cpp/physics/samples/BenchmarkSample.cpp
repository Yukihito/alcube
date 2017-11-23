#include "BenchmarkSample.h"

namespace alcube::physics::samples {
  void BenchmarkSample::runApp(int argc, char **argv) {
    unsigned int maxCellCount = 16384; // 2^14
    //unsigned int maxCellCount = 64;
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

    for (int i = 0; i < 1000; i++) {
      profiler->start(timerId);
      simulator->update(deltaTime);
      profiler->stop(timerId);
    }

    resources->release();

    /*
    int prev = 0;
    for (int i = 0; i < maxCellCount; i++) {
      //auto state = simulator->dtos.currentStates[i];
      auto relation = simulator->dtos.gridAndCellRelations[i];
      //std::cout << "x: " << state.position.x << ", y: " << state.position.y << ", z: " << state.position.z << ", grid: " << state.gridIndex << std::endl;
      std::cout << "grid: " << relation.gridIndex << ", cell: " << relation.cellIndex << std::endl;
      if (prev > relation.gridIndex) {
        std::cout << "Invalid order" << std::endl;
        exit(1);
      }
      prev = relation.gridIndex;
    }
     */

    /*
    int max = 0;
    int indexOfMax = 0;
    int start = 0;
    int end = 0;
    for (int i = 0; i < 64 * 64 * 64; i++) {
      int diff = (simulator->dtos.gridEndIndices[i] - 1) - simulator->dtos.gridStartIndices[i];
      if (diff > max) {
        max = diff;
        indexOfMax = i;
        start = simulator->dtos.gridStartIndices[i];
        end = simulator->dtos.gridEndIndices[i];
      }
      //std::cout << simulator->dtos.gridStartIndices[i] << ", " << simulator->dtos.gridEndIndices[i] << std::endl;
    }
    std::cout << "max: " << max << ", index:" << indexOfMax << ", start: " << start << ", end: " << end << std::endl;
*/
    profiler->update();
    std::cout << "UINT_MAX:" << UINT_MAX << std::endl;
    std::cout << "USHRT_MAX:" << USHRT_MAX << std::endl;
  }
}