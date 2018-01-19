#ifndef ALCUBE_KEYBOARD_H
#define ALCUBE_KEYBOARD_H
#include <unordered_map>

namespace alcube::utils {
  class Keyboard {
    public:
      bool isKeyPressed(int keyCode);
      void onKeyDown(int keyCode);
      void onKeyUp(int keyCode);
    private:
      std::unordered_map<int, bool> pressed;
  };
}

#endif //ALCUBE_KEYBOARD_H
