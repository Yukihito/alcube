#include "Actor.h"

namespace alcube::models {
  using namespace utils::opencl::conversions;

  void Actor::init(alcube::physics::Actor *physicsActor) {
    this->physicsActor = physicsActor;
  }

  void Actor::init(int id) {
    this->id = id;
    this->physicsActor = nullptr;
  }

  int Actor::getId() {
    return id;
  }

  glm::vec3 Actor::getPosition() {
    return toGlm(physicsActor->physicalQuantity.position);
  }

  void Actor::setPosition(glm::vec3 arg) {
    physicsActor->physicalQuantity.position = toCl(arg);

  }

  glm::quat Actor::getRotation() {
    return toGlmQuat(physicsActor->physicalQuantity.rotation);
  }

  void Actor::setRotation(glm::quat arg) {
    physicsActor->physicalQuantity.rotation = toCl(arg);
  }

  glm::vec3 Actor::getLinearMomentum() {
    return toGlm(physicsActor->physicalQuantity.linearMomentum);
  }

  void Actor::setLinearMomentum(glm::vec3 arg) {
    physicsActor->physicalQuantity.linearMomentum = toCl(arg);
  }

  glm::vec3 Actor::getAngularMomentum() {
    return toGlm(physicsActor->physicalQuantity.angularMomentum);
  }

  void Actor::setAngularMomentum(glm::vec3 arg) {
    physicsActor->physicalQuantity.angularMomentum = toCl(arg);
  }

  ActorFactory::ActorFactory(utils::MemoryPool<Actor> *memoryPool) {
    this->memoryPool = memoryPool;
  }

  Actor* ActorFactory::create(physics::Features *feature) {
    auto physicsActor = feature->createPhysicsActor();
    auto actor = memoryPool->get();
    actor->init(physicsActor);
    return actor;
  }

  alcube::physics::Actor* Actor::getPhysicsActor() {
    return physicsActor;
  }
}
