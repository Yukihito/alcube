#include "Actor.h"

namespace alcube::models::actor {
  using namespace utils::opencl::conversions;
  Actor::Actor(int id, int physicsFeaturesId, alcube::physics::Actor* physicsActor) {
    this->id = id;
    this->physicsFeaturesId = physicsFeaturesId;
    this->physicsActor = physicsActor;
  }

  int Actor::getId() {
    return id;
  }

  int Actor::getPhysicsFeaturesId() {
    return physicsFeaturesId;
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

  Actor* ActorRepository::find(int id) {
    return actors[id];
  }

  void ActorRepository::store(Actor *entity) {
    actors[entity->getId()] = entity;
  }

  void ActorRepository::del(int id) {
    actors.erase(id);
  }
}
