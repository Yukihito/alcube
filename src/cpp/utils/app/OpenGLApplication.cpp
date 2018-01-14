#include "OpenGLApplication.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
namespace alcube::utils::app {
  OpenGLApplication* appInst;

  void OpenGLApplication::close() {
    endStatusMutex.lock();
    endStatus = 1;
    endStatusMutex.unlock();
  }

  void OpenGLApplication::drawEvent() {
    appInst->onDraw();
  }

  void OpenGLApplication::drawTimerEvent(int value) {
    appInst->endStatusMutex.lock();
    if (appInst->endStatus == 2) {
      appInst->endStatusMutex.unlock();
      exit(0);
    }
    appInst->endStatusMutex.unlock();
    glutPostRedisplay();
    glutTimerFunc(1000 / appInst->fps, drawTimerEvent, 0);
  }

  void OpenGLApplication::exitEvent() {
    appInst->onClose();
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

  void OpenGLApplication::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    isMultiSampleEnabled = false;
    appName = "";
  }

  void OpenGLApplication::keyEvent(unsigned char key, int x, int y) {
    appInst->keyboard->onKeyDown(key);
    int esc = 27;
    if (key == esc) {
      appInst->close();
    }
  }

  void OpenGLApplication::specialKeyEvent(int key, int x, int y) {
    appInst->keyboard->onSpecialKeyDown(key);
  }

  void OpenGLApplication::specialKeyUpEvent(int key, int x, int y) {
    appInst->keyboard->onSpecialKeyUp(key);
  }

  void OpenGLApplication::keyUpEvent(unsigned char key, int x, int y) {
    appInst->keyboard->onKeyUp(key);
  }

  void OpenGLApplication::motionEvent(int x, int y) {
    appInst->mouse->onMouseMove(x, y);
  }

  void OpenGLApplication::passiveMotionEvent(int x, int y) {
    appInst->mouse->onMouseMove(x, y);
  }

  void OpenGLApplication::mouseEvent(int button, int state,int x, int y) {
  }

  void OpenGLApplication::setupWindow(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    unsigned int displayMode = /*GLUT_3_2_CORE_PROFILE | */GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH;
    if (isMultiSampleEnabled) {
      displayMode = displayMode | GLUT_MULTISAMPLE;
    }
    glutInitDisplayMode(displayMode);
    glutCreateWindow(appName.c_str());
  }

  void OpenGLApplication::setEventListeners() {
    atexit(exitEvent);
    glutDisplayFunc(drawEvent);
    glutKeyboardFunc(keyEvent);
    glutSpecialFunc(specialKeyEvent);
    glutKeyboardUpFunc(keyUpEvent);
    glutSpecialUpFunc(specialKeyUpEvent);
    glutMotionFunc(motionEvent);
    glutPassiveMotionFunc(passiveMotionEvent);
    glutMouseFunc(mouseEvent);
    glutTimerFunc(1000 / fps, drawTimerEvent, 0);
  }

  void OpenGLApplication::printSystemInfo() {
    std::cout << "OpenGL ver. " << glGetString(GL_VERSION) << std::endl;
  }

  void OpenGLApplication::runApp(int argc, char **argv) {
    appInst = this;
    endStatus = 0;
    initWindowParams();
    setupWindow(argc, argv);
    keyboard = new Keyboard();
    mouse = new Mouse();
    onInit();
    setEventListeners();
    std::thread th = std::thread(updateLoop);
    th.detach();
    glutMainLoop();
  }
}
#pragma clang diagnostic pop