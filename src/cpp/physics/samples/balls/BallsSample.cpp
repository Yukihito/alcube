#include "BallsSample.h"

namespace alcube::physics::samples::balls {
  Ball::Ball(drawing::shapes::Shapes *shapes, drawing::shaders::Shaders *shaders) {
    shape = &shapes->points.sphere;
    shader = &shaders->directionalLight;
  }

  /*
  glm::mat4 Ball::getModelMat() {
   return glm::translate(position) * glm::toMat4(rotation) * glm::scale(glm::vec3(radius, radius, radius));
  }
   */

  BallsSample::BallsSample() : ApplicationBase(1600, 1200, 60, "BallsSample") {}

  void BallsSample::add(Ball *ball) {
    drawer->add(ball);
    physicsSimulator->add(ball);
  }

  void BallsSample::onInit() {
    initBase(8, 16384);
    auto ball0 = new Ball(shapes, shaders);
    ball0->position = glm::vec3(0.0f, 0.0f, 0.0f);
    ball0->linearMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
    auto ball1 = new Ball(shapes, shaders);
    ball1->position = glm::vec3(3.0f, 0.0f, 0.0f);
    ball1->linearMomentum = glm::vec3(-5.0f, 0.1f, 1.0f);
    add(ball0);
    add(ball1);
  }
}