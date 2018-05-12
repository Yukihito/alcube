#include "PeriodicScheduler.h"

namespace alcube::utils {
  void PeriodicScheduler::schedule(
    float intervalMs,
    std::function<bool()> terminationCondition,
    std::function<void()> f
  ) {
    while (!terminationCondition()) {
      auto beginTime = std::chrono::system_clock::now();
      f();
      auto endTime = std::chrono::system_clock::now();
      auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
      auto nextFlameInterval = (int)(intervalMs - elapsedTime);
      if (nextFlameInterval > 0) {
        std::chrono::milliseconds sleepingInterval(nextFlameInterval);
        std::this_thread::sleep_for(sleepingInterval);
      }
    }
  }
}
