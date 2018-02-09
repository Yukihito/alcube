#include "OpenGLApplication.h"

namespace alcube::utils::app {
  OpenGLApplication* appInst;

  OpenGLApplication::OpenGLApplication(
    unsigned int windowWidth,
    unsigned int windowHeight,
    unsigned int fps,
    std::string appName
  ) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->fps = fps;
    this->appName = std::move(appName);
    keyboard = new Keyboard();
    endStatus = 0;
  }

  void OpenGLApplication::close() {
    endStatusMutex.lock();
    endStatus = 1;
    endStatusMutex.unlock();
  }

  void OpenGLApplication::drawEvent() {
    appInst->onDraw();
  }

  void OpenGLApplication::exitEvent() {
    appInst->onClose();
    glfwTerminate();
  }

  void OpenGLApplication::updateLoop() {
    while (true) {
      std::chrono::system_clock::time_point updateStartTime = std::chrono::system_clock::now();
      appInst->endStatusMutex.lock();
      if (appInst->endStatus == 1) {
        appInst->endStatusMutex.unlock();
        break;
      }
      appInst->endStatusMutex.unlock();

      appInst->onUpdate();
      std::chrono::system_clock::time_point updateEndTime = std::chrono::system_clock::now();
      int elapsedTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(updateEndTime - updateStartTime).count();
      int nextFlameInterval = (1000 / appInst->fps) - elapsedTime;

      if (nextFlameInterval > 0) {
        std::chrono::milliseconds intervalMs(nextFlameInterval);
        std::this_thread::sleep_for(intervalMs);
      }
    }

    appInst->endStatusMutex.lock();
    appInst->endStatus = 2;
    appInst->endStatusMutex.unlock();
  }

  void OpenGLApplication::keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      appInst->keyboard->onKeyDown(key);
      int esc = 256;
      if (key == esc) {
        appInst->close();
      }
    } else if (action == GLFW_RELEASE) {
      appInst->keyboard->onKeyUp(key);
    }
  }

  void OpenGLApplication::setupWindow(int argc, char **argv) {
    if (!glfwInit()) {
      std::cout << "glfwInit failed." << std::endl;
      exit(1);
    }
    window = glfwCreateWindow(windowWidth, windowHeight, appName.c_str(), nullptr, nullptr);
    if (!window) {
      std::cout << "glfwCreateWindow failed." << std::endl;
      glfwTerminate();
      exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
      std::cout << "glewInit failed : " << glewGetErrorString(err) << std::endl;
    }
  }

  void OpenGLApplication::setEventListeners() {
    atexit(exitEvent);
    glfwSetKeyCallback(window, keyEvent);
  }

  void OpenGLApplication::printSystemInfo() {
    std::cout << "OpenGL ver. " << glGetString(GL_VERSION) << std::endl;
  }

  void OpenGLApplication::runApp(int argc, char **argv) {
    appInst = this;
    setupWindow(argc, argv);
    onInit();
    setEventListeners();
    std::thread th = std::thread(updateLoop);
    th.detach();
    while (!glfwWindowShouldClose(window)) {
      std::chrono::system_clock::time_point drawingStartTime = std::chrono::system_clock::now();
      appInst->endStatusMutex.lock();
      if (appInst->endStatus == 2) {
        appInst->endStatusMutex.unlock();
        exit(0);
      }
      appInst->endStatusMutex.unlock();
      drawEvent();
      glfwSwapBuffers(window);
      glfwPollEvents();
      std::chrono::system_clock::time_point drawingEndTime = std::chrono::system_clock::now();
      int elapsedTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(drawingEndTime - drawingStartTime).count();
      int nextFlameInterval = (1000 / /*appInst->fps*/30) - elapsedTime;
      if (nextFlameInterval > 0) {
        std::chrono::milliseconds intervalMs(nextFlameInterval);
        std::this_thread::sleep_for(intervalMs);
      }
    }
  }
}
