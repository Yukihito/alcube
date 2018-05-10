#include "OpenGLWindow.h"

namespace alcube::utils {
  OpenGLWindow* OpenGLWindow::instance;
  OpenGLWindow::OpenGLWindow(
    unsigned int width,
    unsigned int height,
    const std::string& name
  ) {
    OpenGLWindow::instance = this;
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
    glfwSetKeyCallback(window, keyEvent);
  }

  void OpenGLWindow::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  void OpenGLWindow::close() {
    glfwTerminate();
  }

  bool OpenGLWindow::shouldClose() {
    return glfwWindowShouldClose(window) != 0;
  }

  Keyboard* OpenGLWindow::getKeyboard() {
    return &keyboard;
  }

  void OpenGLWindow::keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      OpenGLWindow::instance->keyboard.onKeyDown(key);
    } else if (action == GLFW_RELEASE) {
      OpenGLWindow::instance->keyboard.onKeyUp(key);
    }
  }
}
