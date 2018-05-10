#include "Keyboard.h"

namespace alcube::utils {
  bool Keyboard::isKeyPressed(int keyCode) {
    return keyStates.count(keyCode) > 0 && keyStates[keyCode];
  }

  void Keyboard::onKeyDown(int keyCode) {
    keyStates[keyCode] = true;
    keyDownEventsMutex.lock();
    keyDownEvents.push_back(keyCode);
    keyDownEventsMutex.unlock();
  }

  void Keyboard::onKeyUp(int keyCode) {
    keyStates[keyCode] = false;
  }

  void Keyboard::update() {
    keyDownEventsMutex.lock();
    for (auto keyDownEvent : keyDownEvents) {
      for (const auto &keyDownCallback: keyDownCallbacks) {
        keyDownCallback(keyDownEvent);
      }
    }
    keyDownEvents.clear();
    keyDownEventsMutex.unlock();
  }

  void Keyboard::addKeyDownEventHandler(const std::function<void(int)> &handler) {
    keyDownCallbacks.push_back(handler);
  }
}
