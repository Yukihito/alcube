#ifndef ALCUBE_KEYBOARD_H
#define ALCUBE_KEYBOARD_H
#include <unordered_map>

namespace alcube::utils {
  class Keyboard {
    public:
      bool isKeyPressed(unsigned char keyCode);
      void onKeyDown(unsigned char keyCode);
      void onKeyUp(unsigned char keyCode);
      bool isSpecialKeyPressed(int keyCode);
      void onSpecialKeyDown(int keyCode);
      void onSpecialKeyUp(int keyCode);
    private:
      std::unordered_map<unsigned char, bool> pressed;
      std::unordered_map<int, bool> pressedSpecial;
  };
}

#endif //ALCUBE_KEYBOARD_H
