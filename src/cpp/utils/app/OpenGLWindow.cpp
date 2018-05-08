#include "OpenGLWindow.h"

namespace alcube::utils::app {
  OpenGLWindow* OpenGLWindow::instance;
  OpenGLWindow::OpenGLWindow(
    std::function<void()> drawCallback,
    std::function<void()> updateCallback,
    std::function<void()> closeCallback,
    unsigned int width,
    unsigned int height,
    unsigned int fps,
    float updateInterval,
    std::string name
  ) {
    OpenGLWindow::instance = this;
    this->drawCallback = std::move(drawCallback);
    this->updateCallback = std::move(updateCallback);
    this->closeCallback = std::move(closeCallback);
    this->keyboard = new Keyboard();
    this->fps = fps;
    this->updateInterval = updateInterval;
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

  void OpenGLWindow::run() {
    glfwSetKeyCallback(window, keyEvent);
    auto th = std::thread([&]{
      callPeriodically(updateCallback, updateInterval * 1000.0f);
    });
    callPeriodically([&] {
      drawCallback();
      glfwSwapBuffers(window);
      glfwPollEvents();
    }, 1000.0f / fps);
    glfwTerminate();
    th.join();
    closeCallback();
  }

  void OpenGLWindow::callPeriodically(std::function<void()> f, float interval) {
    while (!glfwWindowShouldClose(window)) {
      auto beginTime = std::chrono::system_clock::now();
      f();
      auto endTime = std::chrono::system_clock::now();
      auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
      auto nextFlameInterval = (int)(interval - elapsedTime);
      if (nextFlameInterval > 0) {
        std::chrono::milliseconds intervalMs(nextFlameInterval);
        std::this_thread::sleep_for(intervalMs);
      }
    }
  }

  void OpenGLWindow::keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      OpenGLWindow::instance->keyboard->onKeyDown(key);
    } else if (action == GLFW_RELEASE) {
      OpenGLWindow::instance->keyboard->onKeyUp(key);
    }
  }
}
