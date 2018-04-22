#include "Settings.h"

namespace alcube::scripting::mappings {
  using namespace utils;
  void PhysicsSettings::init() {
    Prototype::init();
    define<float, timeStepSize>();
    define<float, gravity>();
  }

  void WindowSettings::init() {
    Prototype::init();
    define<int, width>();
    define<int, height>();
  }

  void WorldSettings::init() {
    Prototype::init();
    define<float, size>();
    define<int, maxActorCount>();
  }

  void Settings::init() {
    Prototype::init();
    define<int, fps>();
  }
}
