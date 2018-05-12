#ifndef ALCUBE_PERIODICSCHEDULER_H
#define ALCUBE_PERIODICSCHEDULER_H

#include <functional>
#include <thread>
#include <utility>

namespace alcube::utils {
  class PeriodicScheduler {
    public:
      void schedule(
        float intervalMs,
        std::function<bool()> terminationCondition,
        std::function<void()> f
      );
  };
}

#endif //ALCUBE_PERIODICSCHEDULER_H
