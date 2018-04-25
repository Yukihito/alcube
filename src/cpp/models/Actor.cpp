#include "Actor.h"

namespace alcube::models {
  using namespace utils::opencl::conversions;

  void Actor::init(
    int id,
    alcube::physics::Actor *physicsActor,
    models::drawing::InstanceRenderer* instanceRenderer
  ) {
    this->id = id;
    this->physicsActor = nullptr;
    this->physicsActor = physicsActor;
    this->instanceRenderer = instanceRenderer;
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

  glm::vec3 Actor::getColor() {
    return instanceRenderer->getColor();
  }

  void Actor::setColor(glm::vec3 arg) {
    instanceRenderer->setColor(arg);
  }

  unsigned int Actor::getIndex() {
    return physicsActor->index;
  }

  ActorFactory::ActorFactory(
    utils::MemoryPool<Actor> *memoryPool,
    drawing::InstanceRendererFactory* instanceRendererFactory
  ) {
    this->memoryPool = memoryPool;
    this->instanceRendererFactory = instanceRendererFactory;
  }

  Actor* ActorFactory::create(
    physics::Features *feature,
    models::drawing::Renderer* renderer
  ) {
    auto physicsActor = feature->createPhysicsActor();
    auto actor = memoryPool->get();
    auto instanceRenderer = instanceRendererFactory->create(actor, renderer);
    int nextId = instanceCount;
    actor->init(nextId, physicsActor, instanceRenderer);
    instanceCount++;
    return actor;
  }

  alcube::physics::Actor* Actor::getPhysicsActor() {
    return physicsActor;
  }
}
