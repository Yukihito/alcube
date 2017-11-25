#include "Keyboard.h"

namespace alcube::utils {
  bool Keyboard::isKeyPressed(unsigned char keyCode) {
    return pressed[keyCode];
  }

  void Keyboard::onKeyDown(unsigned char keyCode) {
    pressed[keyCode] = true;
  }

  void Keyboard::onKeyUp(unsigned char keyCode) {
    pressed[keyCode] = false;
  }

  bool Keyboard::isSpecialKeyPressed(int keyCode) {
    return pressedSpecial[keyCode];
  }

  void Keyboard::onSpecialKeyDown(int keyCode) {
    pressedSpecial[keyCode] = true;
  }

  void Keyboard::onSpecialKeyUp(int keyCode) {
    pressedSpecial[keyCode] = false;
  }
}