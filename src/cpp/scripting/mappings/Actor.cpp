#include "Actor.h"

namespace alcube::scripting::mappings {
  using namespace utils;

  void Actor::init() {
    Prototype::init();
    define<glm::vec3, position>();
    define<glm::quat, rotation>();
    define<glm::vec3, linearMomentum>();
    define<glm::vec3, angularMomentum>();
  }

  glm::vec3 Actor::position::get(models::Actor *actor) {
    return actor->getPosition();
  }

  void Actor::position::set(models::Actor* actor, glm::vec3 v) {
    actor->setPosition(v);
  }

  glm::quat Actor::rotation::get(alcube::models::Actor* actor) {
    return actor->getRotation();
  }

  void Actor::rotation::set(alcube::models::Actor* actor, glm::quat v) {
    actor->setRotation(v);
  }

  glm::vec3 Actor::linearMomentum::get(alcube::models::Actor* actor) {
    return actor->getLinearMomentum();
  }

  void Actor::linearMomentum::set(alcube::models::Actor* actor, glm::vec3 v) {
    actor->setLinearMomentum(v);
  }

  glm::vec3 Actor::angularMomentum::get(alcube::models::Actor* actor) {
    return actor->getAngularMomentum();
  }

  void Actor::angularMomentum::set(alcube::models::Actor* actor, glm::vec3 v) {
    actor->setAngularMomentum(v);
  }

  ActorFactory::ActorFactory(models::ActorFactory *underlying) : SingletonPrototype<models::ActorFactory>(underlying) {}

  void ActorFactory::init() {
    Prototype::init();
    defineMethod("create", create);
  }

  void ActorFactory::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    if (info.Length() < 1) {
      info.GetReturnValue().Set(v8::Undefined(isolate));
      return;
    }
    auto features = getUnderlying<models::physics::fluid::Features>(info[0]);
    auto underlying = self(info)->create(features);
    auto actor = Actor::instance->objectTemplate->NewInstance();
    actor->SetInternalField(0, v8::External::New(isolate, underlying));
    info.GetReturnValue().Set(actor);
  }
}
