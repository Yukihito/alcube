#include "Keyboard.h"

namespace alcube::utils {
  bool Keyboard::isKeyPressed(int keyCode) {
    return pressed.count(keyCode) > 0 && pressed[keyCode];
  }

  bool Keyboard::isKeyTyped(int keyCode) {
    return typed.count(keyCode) > 0 && typed[keyCode];
  }

  void Keyboard::onKeyDown(int keyCode) {
    typed[keyCode] = !isKeyPressed(keyCode);
    pressed[keyCode] = true;
  }

  void Keyboard::onKeyUp(int keyCode) {
    pressed[keyCode] = false;
    typed[keyCode] = false;
  }
}