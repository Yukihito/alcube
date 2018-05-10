#ifndef ALCUBE_OPENGLAPPLICATION_H
#define ALCUBE_OPENGLAPPLICATION_H

#include <GL/glew.h>
#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include "Mouse.h"
#include "Keyboard.h"
#include <GLFW/glfw3.h>

namespace alcube::utils {
  class OpenGLWindow {
    public:
      explicit OpenGLWindow(
        unsigned int width,
        unsigned int height,
        const std::string& name
      );
      void update();
      void close();
      bool shouldClose();
      Keyboard* getKeyboard();
    private:
      static OpenGLWindow* instance;
      Keyboard keyboard;
      GLFWwindow* window;
      static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
  };
}

#endif //ALCUBE_OPENGLAPPLICATION_H
