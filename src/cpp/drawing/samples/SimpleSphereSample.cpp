#include "SimpleSphereSample.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
namespace alcube::drawing::samples {
  SimpleSphereSample* appInst;

  SimpleSphere::SimpleSphere(glm::vec3& position, shapes::Shapes* shapes, shaders::Shaders* shaders) {
    shape = &shapes->points.sphere;
    shader = &shaders->simple;
    this->position = position;
  }

  glm::mat4 SimpleSphere::getModelMat() {
    return glm::translate(position);
  }

  void SimpleSphereSample::drawEvent() {
    appInst->drawer->draw();
  }

  void SimpleSphereSample::keyEvent(unsigned char key, int x, int y) {
    int esc = 27;
    if (key == esc) {
      exit(0);
    }
  }

  void SimpleSphereSample::setupWindow(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("SimpleSphereSample");
  }

  void SimpleSphereSample::setEventListeners() {
    glutDisplayFunc(drawEvent);
    glutKeyboardFunc(keyEvent);
  }

  void SimpleSphereSample::runApp(int argc, char **argv) {
    appInst = this;
    setupWindow(argc, argv);
    auto shaders = new shaders::Shaders(new utils::FileUtil());
    auto shapes = new shapes::Shapes();
    camera = new Camera(
      glm::vec3(0.0f, 0.0f, 20.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)WINDOW_WIDTH,
      (float)WINDOW_HEIGHT,
      0.1f,
      100.0f
    );
    drawer = new Drawer(camera, &drawablesMutex);
    glm::vec3 pos1 = glm::vec3();
    glm::vec3 pos2 = glm::vec3(5.0f, -4.0f, 0.0f);
    drawer->add(new SimpleSphere(pos1, shapes, shaders));
    drawer->add(new SimpleSphere(pos2, shapes, shaders));
    setEventListeners();
    glutMainLoop();
  }
}

#pragma clang diagnostic pop