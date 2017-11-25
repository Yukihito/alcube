#include "SimpleSphereSample.h"


namespace alcube::drawing::samples {
  SimpleSphere::SimpleSphere(glm::vec3& position, shapes::Shapes* shapes, shaders::Shaders* shaders) {
    shape = &shapes->points.sphere;
    shader = &shaders->simple;
    this->position = position;
  }

  glm::mat4 SimpleSphere::getModelMat() {
    return glm::translate(position);
  }

  void SimpleSphereSample::initWindowParams() {
    windowWidth = 800;
    windowHeight = 600;
    fps = 30;
    appName = "SimpleSphereSample";
  }

  void SimpleSphereSample::onInit() {
    printSystemInfo();
    auto shaders = new shaders::Shaders(new utils::FileUtil());
    auto shapes = new shapes::Shapes();
    camera = new Camera(
      glm::vec3(0.0f, 0.0f, 20.0f),
      glm::quat(),
      glm::radians(45.0f),
      (float)windowWidth,
      (float)windowHeight,
      0.1f,
      100.0f
    );
    drawer = new Drawer(camera, &drawablesMutex);
    glm::vec3 pos1 = glm::vec3();
    glm::vec3 pos2 = glm::vec3(5.0f, -4.0f, 0.0f);
    drawer->add(new SimpleSphere(pos1, shapes, shaders));
    drawer->add(new SimpleSphere(pos2, shapes, shaders));
  }

  void SimpleSphereSample::onDraw() {
    drawer->draw();
  }

  void SimpleSphereSample::onUpdate() {}

  void SimpleSphereSample::onClose() {}
}