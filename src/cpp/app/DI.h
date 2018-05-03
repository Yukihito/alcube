#ifndef ALCUBE_DEPENDENCYINJECTOR_H
#define ALCUBE_DEPENDENCYINJECTOR_H

#include <unordered_map>
#include <unordered_set>
#include <cxxabi.h>
#include <string>
#include <iostream>
#include "../gpu/GPUAccessor.h"
#include "../drawing/Camera.h"
#include "../models/Settings.h"
#include "../drawing/Canvas.h"
#include "../drawing/CanvasWithProfiler.h"
#include "../utils/Profiler.h"
#include "../drawing/shaders/Shaders.h"
#include "Grid.h"
#include "../utils/alcubemath.h"
#include "../physics/softbody/Simulator.h"
#include "../physics/fluid/Simulator.h"
#include "../physics/Simulator.h"
#include "../models/Alcube.h"
#include "../models/physics/fluid/Features.h"
#include "../models/physics/softbody/Features.h"

namespace alcube::app {
  class DI {
    public:
      template <class T>
      T* get();
    private:
      template <class T>
      T* inject();
      std::unordered_map<std::string, void*> instances;
      std::unordered_set<std::string> injectionStartedKeys;
  };

  template <class T>
  T* DI::get() {
    int stat;
    std::string classTag = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &stat);
    bool started = injectionStartedKeys.count(classTag) > 0;
    bool finished = instances.count(classTag) > 0;
    if (started && !finished) {
      std::cout << "Circular dependency: " << classTag << std::endl;
      exit(1);
    }
    if (!started && !finished) {
      injectionStartedKeys.emplace(classTag);
      instances[classTag] = inject<T>();
    }
    return (T*)instances[classTag];
  }
}

#endif //ALCUBE_DEPENDENCYINJECTOR_H
