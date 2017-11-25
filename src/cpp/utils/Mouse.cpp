#include "Mouse.h"

namespace alcube::utils {
  void Mouse::update() {
    dx = x - lastX;
    dy = y - lastY;
    lastX = x;
    lastY = y;
  }

  void Mouse::onMouseMove(int x, int y) {
    this->x = x;
    this->y = y;
  }
}