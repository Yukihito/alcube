#include "Alcube.h"

namespace alcube::scripting::mappings {
  using namespace utils;

  void Alcube::init() {
    Prototype::init();
    define<int, ActorCount>();
  }
}
