#include "drawing/samples/SimpleSphereSample.h"
#include "physics/samples/BenchmarkSample.h"
#include "physics/samples/balls/BallsSample.h"
#include "physics/samples/particles/ParticlesSample.h"
#include "physics/samples/balls/ManyBallsSample.h"
#include <iostream>

void printVec3(glm::vec3& v) {
  std::cout << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << std::endl;
}

int main(int argc, char * argv[]) {
  //auto sample = new alcube::drawing::samples::SimpleSphereSample();
  //auto sample = new alcube::physics::samples::BenchmarkSample();
  //auto sample = new alcube::physics::samples::balls::BallsSample();
  //auto sample = new alcube::physics::samples::particles::ParticlesSample();
  auto sample = new alcube::physics::samples::balls::ManyBallsSample();
  sample->runApp(argc, argv);
  /*
  glm::vec3 v1 = glm::vec3(1.0f, 2.0f, 3.0f);
  glm::vec3 v2 = glm::vec3(1.0f, 1000.0f, 3.0f);
  glm::vec3 v3 = glm::vec3(0.0f, -1.0f, 0.0f);
  glm::vec3 v4 = glm::cross(v1, v3);
  glm::vec3 v5 = glm::cross(v2, v3);
  std::cout << "x: " << v4.x << ", y: " << v4.y << ", z: " << v4.z << std::endl;
  std::cout << "x: " << v5.x << ", y: " << v5.y << ", z: " << v5.z << std::endl;
   */
  //float deltaTime = 1.0f / 30.0f;
  float mass = 3.0f;
  float radius = 1.0f;
  glm::vec3 workingPoint = glm::vec3(0.0f, -radius, 0.0f);
  glm::vec3 linearVelocity = glm::vec3(1.0f, 0.0f, 9.0f);
  glm::vec3 linearMomentum = linearVelocity * mass;
  float momentOfInertia = mass * radius * radius * (2.0f / 5.0f);
  glm::vec3 angularMomentum = glm::cross(workingPoint, glm::vec3(-3.0f, 0.0f, 0.4f));
  glm::vec3 angularVelocity = angularMomentum / momentOfInertia;
  printVec3(angularVelocity);
  glm::vec3 rotationAxis = glm::normalize(angularVelocity);
  printVec3(rotationAxis);
  //glm::vec3 angularMomentum = angularVelocity * momentOfInertia;

  glm::vec3 collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 relationalVelocity = linearVelocity + glm::cross(angularVelocity, workingPoint);
  relationalVelocity -= collisionNormal * glm::dot(collisionNormal, relationalVelocity);
  printVec3(relationalVelocity);

  glm::vec3 impulse = -(2.0f / 7.0f) * mass * (linearVelocity + glm::cross(-workingPoint, angularVelocity));
  printVec3(impulse);
  glm::vec3 linearMomentum2 = linearMomentum + impulse;
  glm::vec3 linearVelocity2 = linearMomentum2 / mass;
  glm::vec3 angularMomentum2 = angularMomentum + glm::cross(workingPoint, impulse);
  glm::vec3 angularVelocity2 = angularMomentum2 / momentOfInertia;
  glm::vec3 relationalVelocity2 = linearVelocity2 + glm::cross(angularVelocity2, workingPoint);
  relationalVelocity2 -= collisionNormal * glm::dot(collisionNormal, relationalVelocity2);
  printVec3(relationalVelocity2);

  /*
  glm::vec3 impulse = glm::vec3(3.0f, -4.5f, 0.2f);
  glm::vec3 normal = glm::normalize(glm::vec3(-1.2f, 3.5f, 1.7f));
  impulse -= glm::dot(impulse, normal) * normal;
  printVec3(impulse);
  glm::vec3 tmp = glm::cross(normal, glm::cross(impulse, normal));
  printVec3(tmp);
   */
  return 0;
}