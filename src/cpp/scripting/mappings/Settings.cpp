#include "Settings.h"

namespace alcube::scripting::mappings {
  using namespace utils;
  //PhysicsSettings::PhysicsSettings(alcube::models::PhysicsSettings *underlying) : SingletonPrototype(underlying) {}
  void PhysicsSettings::init() {
    Prototype::init();
    define<float, timeStepSize>();
    define<float, gravity>();
  }

  //WindowSettings::WindowSettings(alcube::models::WindowSettings *underlying) : SingletonPrototype(underlying) {}

  void WindowSettings::init() {
    Prototype::init();
    define<int, width>();
    define<int, height>();
  }

  //WorldSettings::WorldSettings(alcube::models::WorldSettings *underlying) : SingletonPrototype(underlying) {}

  void WorldSettings::init() {
    Prototype::init();
    define<float, size>();
    define<int, maxActorCount>();
  }

  //Settings::Settings(alcube::models::Settings *underlying) : SingletonPrototype(underlying) {}

  void Settings::init() {
    Prototype::init();
    define<int, fps>();
  }
}
