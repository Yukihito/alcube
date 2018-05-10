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
        std::function<void()> drawCallback,
        std::function<void()> updateCallback,
        std::function<void()> closeCallback,
        unsigned int width,
        unsigned int height,
        unsigned int fps,
        float updateInterval,
        std::string name
      );
      void run();
      void update();
      void close();
      Keyboard* getKeyboard();
    private:
      static OpenGLWindow* instance;
      Keyboard* keyboard = nullptr;
      unsigned int fps;
      float updateInterval;
      GLFWwindow* window;
      std::function<void()> drawCallback;
      std::function<void()> updateCallback;
      std::function<void()> closeCallback;
      static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
      void callPeriodically(std::function<void()> f, float interval);
  };
}

#endif //ALCUBE_OPENGLAPPLICATION_H
