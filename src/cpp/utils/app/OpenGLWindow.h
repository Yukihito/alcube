#ifndef ALCUBE_OPENGLAPPLICATION_H
#define ALCUBE_OPENGLAPPLICATION_H

#include <GL/glew.h>
#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include "../Mouse.h"
#include "../Keyboard.h"
#include <GLFW/glfw3.h>

namespace alcube::utils::app {
  enum WindowClosingStatus {
    NONE,
    PROCESSING,
    FINISHED
  };

  class OpenGLWindow {
    public:
      static OpenGLWindow* instance;
      explicit OpenGLWindow(
        std::function<void()> drawCallback,
        std::function<void()> updateCallback,
        std::function<void()> closeCallback
      );
      void setup(
        unsigned int width,
        unsigned int height,
        unsigned int fps,
        float updateInterval,
        std::string name
      );
      void run();
      bool isClosed();
    protected:
      void close();
      void printSystemInfo();
    private:
      Keyboard* keyboard = nullptr;
      unsigned int fps;
      float updateInterval;
      std::function<void()> drawCallback;
      std::function<void()> updateCallback;
      std::function<void()> closeCallback;

      static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
      WindowClosingStatus closingStatus;
      GLFWwindow* window;
      void updateLoop();
  };
}

#endif //ALCUBE_OPENGLAPPLICATION_H
