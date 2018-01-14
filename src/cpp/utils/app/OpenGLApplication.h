#ifndef ALCUBE_OPENGLAPPLICATION_H
#define ALCUBE_OPENGLAPPLICATION_H

#include <mutex>
#include <cstdlib>
#include <GL/glut.h>
//#include <GLUT/glut.h>
#include <string>
#include <thread>
#include <iostream>
#include "Application.h"
#include "../Mouse.h"
#include "../Keyboard.h"

namespace alcube::utils::app {
  class OpenGLApplication : public Application {
    public:
      void runApp(int argc, char **argv) override;
    protected:
      unsigned int windowWidth = 800;
      unsigned int windowHeight = 600;
      unsigned int fps = 30;
      bool isMultiSampleEnabled = false;
      std::string appName = "";

      Keyboard* keyboard = nullptr;
      Mouse* mouse = nullptr;

      virtual void initWindowParams();
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
      static void updateLoop();
      static void exitEvent();
      static void drawTimerEvent(int value);
      static void keyEvent(unsigned char key, int x, int y);
      static void keyUpEvent(unsigned char key, int x, int y);
      static void specialKeyEvent(int key, int x, int y);
      static void specialKeyUpEvent(int key, int x, int y);
      static void mouseEvent(int button,int state,int x,int y);
      static void motionEvent(int x, int y);
      static void passiveMotionEvent(int x, int y);
      int endStatus;
      std::mutex endStatusMutex;
  };
}

#endif //ALCUBE_OPENGLAPPLICATION_H
