#include "Actor.h"

namespace alcube::models {
  using namespace utils::opencl::conversions;

  void Actor::init(
    int id,
    alcube::physics::Actor *physicsActor,
    models::drawing::Model3D* model3D
  ) {
    this->id = id;
    this->physicsActor = nullptr;
    this->physicsActor = physicsActor;
    this->model3D = model3D;
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
    return model3D->getColor();
  }

  void Actor::setColor(glm::vec3 arg) {
    model3D->setColor(arg);
  }

  unsigned int Actor::getIndex() {
    return physicsActor->index;
  }

  alcube::models::drawing::Model3D* Actor::getModel3D() {
    return model3D;
  }

  ActorFactory::ActorFactory(
    utils::MemoryPool<Actor> *memoryPool,
    drawing::Model3DFactory* model3DFactory
  ) {
    this->memoryPool = memoryPool;
    this->model3DFactory = model3DFactory;
  }

  Actor* ActorFactory::create(physics::Features *feature) {
    auto physicsActor = feature->createPhysicsActor();
    auto actor = memoryPool->get();
    auto entity = model3DFactory->create(actor);
    int nextId = instanceCount;
    actor->init(nextId, physicsActor, entity);
    instanceCount++;
    return actor;
  }

  alcube::physics::Actor* Actor::getPhysicsActor() {
    return physicsActor;
  }
}
