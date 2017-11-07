#include "Shape.h"

namespace alcube::drawing {
  void Shape::initialize() {
    this->buffer = createBuffer();
  }
}
