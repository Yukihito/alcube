#include "BallsSample.h"

namespace alcube::physics::samples::balls {
  Ball::Ball(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders) {
    shape = &shapes->points.sphere;
    shader = &shaders->simple;
  }

  glm::mat4 Ball::getModelMat() {
    return glm::translate(position);
  }

  void BallsSample::initWindowParams() {}

  void BallsSample::onInit() {}

  void BallsSample::onDraw() {}

  void BallsSample::onUpdate() {}

  void BallsSample::onClose() {}
}