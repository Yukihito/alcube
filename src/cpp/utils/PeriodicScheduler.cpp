#include "PeriodicScheduler.h"

namespace alcube::utils {
  void PeriodicScheduler::schedule(
    float intervalMs,
    std::function<bool()> terminationCondition,
    std::function<void()> f
  ) {
    callPeriodically(intervalMs, std::move(terminationCondition), std::move(f));
  }

  std::thread PeriodicScheduler::scheduleAsync(
    float intervalMs,
    std::function<bool()> terminationCondition,
    std::function<void()> f
  ) {
    return std::thread([&]{
      callPeriodically(intervalMs, std::move(terminationCondition), std::move(f));
    });
  }

  void PeriodicScheduler::callPeriodically(
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
