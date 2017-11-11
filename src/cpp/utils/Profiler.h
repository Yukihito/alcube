#ifndef ALCUBE_PROFILER_H
#define ALCUBE_PROFILER_H

#include <chrono>
#include <string>
#include <vector>
#include <iostream>

namespace alcube::utils {
  class ProfilingItem {
    public:
      std::chrono::system_clock::time_point startedTime;
      std::string name;
      std::vector<int> intervals;
      int max;
      int min;
      int avg;
      int count;
      void update();
      void show();
  };

  class Profiler {
    public:
      explicit Profiler();
      bool enabled;
      int create(const char* name);
      void start(int id);
      void stop(int id);
      void setShowInterval(int msec);
      void update();
    private:
      int showInterval;
      std::chrono::system_clock::time_point lastShowedTime;
      std::vector<ProfilingItem *> items;
  };
}

#endif //ALCUBE_PROFILER_H
