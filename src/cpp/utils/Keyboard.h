#ifndef ALCUBE_KEYBOARD_H
#define ALCUBE_KEYBOARD_H
#include <unordered_map>
#include <vector>
#include <mutex>

namespace alcube::utils {
  class Keyboard {
    public:
      bool isKeyPressed(int keyCode);
      void onKeyDown(int keyCode);
      void onKeyUp(int keyCode);
      void update();
      void addKeyDownEventHandler(const std::function<void(int)> &handler);
    private:
      std::unordered_map<int, bool> keyStates = {};
      std::vector<int> keyDownEvents = {};
      std::vector<std::function<void(int)>> keyDownCallbacks = {};
      std::mutex keyDownEventsMutex = {};
  };
}

#endif //ALCUBE_KEYBOARD_H
