#include "Keyboard.h"

namespace alcube::utils {
  bool Keyboard::isKeyPressed(int keyCode) {
    return pressed[keyCode];
  }

  void Keyboard::onKeyDown(int keyCode) {
    pressed[keyCode] = true;
  }

  void Keyboard::onKeyUp(int keyCode) {
    pressed[keyCode] = false;
  }
}