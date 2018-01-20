#include "Cell.h"

namespace alcube::physics {
  Cell::Cell() : Sphere() {
    alterEgo = nullptr;
    radiusForAlterEgo = 0.0f;
  }
}