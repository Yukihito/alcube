#include "Actor.h"

namespace alcube::models {
  using namespace utils::opencl::conversions;

  void Actor::init(int id) {
    this->id = id;
    this->physicsActor = nullptr;
    this->model3D = nullptr;
  }

  void Actor::setPhysicsActor(alcube::physics::Actor *physicsActor) {
    this->physicsActor = physicsActor;
  }

  void Actor::setModel3D(alcube::models::drawing::Model3D *model3D) {
    this->model3D = model3D;
  }

  int Actor::getId() {
    return id;
  }

  glm::vec3 Actor::getPosition() {
    return toGlm(physicsActor->getPhysicalQuantityDto()->position);
  }

  void Actor::setPosition(glm::vec3 arg) {
    physicsActor->getPhysicalQuantityDto()->position = toCl(arg);

  }

  glm::quat Actor::getRotation() {
    return toGlmQuat(physicsActor->getPhysicalQuantityDto()->rotation);
  }

  void Actor::setRotation(glm::quat arg) {
    physicsActor->getPhysicalQuantityDto()->rotation = toCl(arg);
  }

  glm::vec3 Actor::getLinearMomentum() {
    return toGlm(physicsActor->getPhysicalQuantityDto()->linearMomentum);
  }

  void Actor::setLinearMomentum(glm::vec3 arg) {
    physicsActor->getPhysicalQuantityDto()->linearMomentum = toCl(arg);
  }

  glm::vec3 Actor::getAngularMomentum() {
    return toGlm(physicsActor->getPhysicalQuantityDto()->angularMomentum);
  }

  void Actor::setAngularMomentum(glm::vec3 arg) {
    physicsActor->getPhysicalQuantityDto()->angularMomentum = toCl(arg);
  }

  glm::vec3 Actor::getColor() {
    return model3D->getColor();
  }

  void Actor::setColor(glm::vec3 arg) {
    model3D->setColor(arg);
  }

  unsigned int Actor::getIndex() {
    return physicsActor->getIndex();
  }

  alcube::models::drawing::Model3D* Actor::getModel3D() {
    return model3D;
  }

  ActorFactory::ActorFactory(utils::MemoryPool<Actor> *memoryPool) {
    this->memoryPool = memoryPool;
  }

  Actor* ActorFactory::create(physics::Features *feature, drawing::RenderingGroup* renderingGroup) {
    auto physicsActor = feature->createPhysicsActor();
    auto actor = memoryPool->get();
    int nextId = instanceCount;
    actor->init(nextId);
    actor->setPhysicsActor(physicsActor);
    auto m = renderingGroup->createModel3D(actor);
    //actor->setModel3D();
    instanceCount++;
    return actor;
  }

  alcube::physics::Actor* Actor::getPhysicsActor() {
    return physicsActor;
  }
}
