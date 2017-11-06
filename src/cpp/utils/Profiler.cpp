#include "Profiler.h"

namespace utils {
  void ProfilingItem::update() {
    count = (int)intervals.size();
    max = 0;
    min = 65535;
    int sum = 0;
    for (int interval: intervals) {
      sum += interval;
      if (max < interval) {
        max = interval;
      }
      if (min > interval) {
        min = interval;
      }
    }
    if (count > 0) {
      avg = sum / count;
    } else {
      avg = 0;
    }
    intervals.clear();
  }

  void ProfilingItem::show() {
    std::cout << name << ":\tcount: " << count << ",\tavg: " << avg << ",\tmax: " << max << ",\tmin: " << min << std::endl;
  }

  int Profiler::create(std::string& name) {
    auto id = (int)items.size();
    auto item = new ProfilingItem();
    item->name = name;
    items.push_back(item);
    return id;
  }

  void Profiler::start(int id) {
    items[id]->startedTime = std::chrono::system_clock::now();
  }

  void Profiler::stop(int id) {
    if (enabled) {
      std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
      int interval = (int) std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - items[id]->startedTime).count();
      items[id]->intervals.push_back(interval);
    }
  }

  void Profiler::setShowInterval(int msec) {
    showInterval = msec;
  }

  void Profiler::update() {
    if (enabled) {
      std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
      int interval = (int) std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastShowedTime).count();
      if (interval >= showInterval) {
        for (ProfilingItem *item: items) {
          item->update();
          item->show();
        }
        lastShowedTime = currentTime;
      }
    }
  }
}
