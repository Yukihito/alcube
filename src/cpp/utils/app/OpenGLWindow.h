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
      explicit OpenGLWindow(std::function<void()> draw);
      void setup(
        unsigned int width,
        unsigned int height,
        unsigned int fps,
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
      std::function<void()> draw;
      static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
      WindowClosingStatus closingStatus;
      GLFWwindow* window;
  };
}

#endif //ALCUBE_OPENGLAPPLICATION_H