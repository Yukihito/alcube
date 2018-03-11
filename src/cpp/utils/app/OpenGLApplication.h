#ifndef ALCUBE_OPENGLAPPLICATION_H
#define ALCUBE_OPENGLAPPLICATION_H

#include <GL/glew.h>
#include <mutex>
#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include "Application.h"
#include "../Mouse.h"
#include "../Keyboard.h"
#include <GLFW/glfw3.h>

namespace alcube::utils::app {
  class OpenGLApplication : public Application {
    public:
      void runApp(int argc, char **argv) override;
      explicit OpenGLApplication(
        unsigned int windowWidth,
        unsigned int windowHeight,
        unsigned int fps,
        std::string appName
      );
    protected:
      unsigned int windowWidth;
      unsigned int windowHeight;
      unsigned int fps;
      std::string appName;
      Keyboard* keyboard = nullptr;
      virtual void onInit() = 0;
      virtual void onUpdate() = 0;
      virtual void onDraw() = 0;
      virtual void onClose() = 0;
      void close();
      void printSystemInfo();
    private:
      void setupWindow(int argc, char* argv[]);
      void setEventListeners();
      static void drawEvent();
      static void exitEvent();
      static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
      static void updateLoop();
      int endStatus;
      std::mutex endStatusMutex;
      GLFWwindow* window;
  };
}

#endif //ALCUBE_OPENGLAPPLICATION_H
