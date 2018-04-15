#include "OpenGLWindow.h"

namespace alcube::utils::app {
  OpenGLWindow* OpenGLWindow::instance;
  OpenGLWindow::OpenGLWindow(void (*draw)()) {
    OpenGLWindow::instance = this;
    fps = 30;
    this->draw = draw;
    keyboard = new Keyboard();
    closingStatus = WindowClosingStatus::NONE;
  }

  bool OpenGLWindow::isClosed() {
    return closingStatus == WindowClosingStatus::FINISHED;
  }

  void OpenGLWindow::close() {
    closingStatus = WindowClosingStatus::PROCESSING;
  }

  void OpenGLWindow::keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      OpenGLWindow::instance->keyboard->onKeyDown(key);
      int esc = 256;
      if (key == esc) {
        OpenGLWindow::instance->close();
      }
    } else if (action == GLFW_RELEASE) {
      OpenGLWindow::instance->keyboard->onKeyUp(key);
    }
  }

  void OpenGLWindow::setup(
    unsigned int width,
    unsigned int height,
    unsigned int fps,
    std::string name
  ) {
    this->fps = fps;
    if (!glfwInit()) {
      std::cout << "glfwInit failed." << std::endl;
      exit(1);
    }
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
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

  void OpenGLWindow::printSystemInfo() {
    std::cout << "OpenGL ver. " << glGetString(GL_VERSION) << std::endl;
  }

  void OpenGLWindow::run() {
    glfwSetKeyCallback(window, keyEvent);

    while (closingStatus == WindowClosingStatus::NONE) {
      std::chrono::system_clock::time_point drawingStartTime = std::chrono::system_clock::now();
      draw();
      glfwSwapBuffers(window);
      glfwPollEvents();

      std::chrono::system_clock::time_point drawingEndTime = std::chrono::system_clock::now();
      int elapsedTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(drawingEndTime - drawingStartTime).count();
      int nextFlameInterval = (1000 / fps) - elapsedTime;
      if (nextFlameInterval > 0) {
        std::chrono::milliseconds intervalMs(nextFlameInterval);
        std::this_thread::sleep_for(intervalMs);
      }

      if (glfwWindowShouldClose(window)) {
        closingStatus = WindowClosingStatus::PROCESSING;
      }
    }
    closingStatus = WindowClosingStatus::FINISHED;
  }
}
