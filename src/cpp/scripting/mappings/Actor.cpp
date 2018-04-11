#include "Actor.h"

namespace alcube::scripting::mappings {
  using namespace utils;
  namespace Actor {
    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::Actor, glm::vec3, fields::position>::define(this);
      Accessor<models::Actor, glm::quat, fields::rotation>::define(this);
      Accessor<models::Actor, glm::vec3, fields::linearMomentum>::define(this);
      Accessor<models::Actor, glm::vec3, fields::angularMomentum>::define(this);
    }
  }

  namespace ActorFactory {
    Prototype::Prototype(models::ActorFactory *underlying) : SingletonPrototype<models::ActorFactory>(underlying) {}

    void Prototype::init() {
      utils::Prototype::init();
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "create"),
        v8::FunctionTemplate::New(isolate, Prototype::create)
      );
    }

    void Prototype::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      v8::HandleScope scope(isolate);
      if (info.Length() < 1) {
        info.GetReturnValue().Set(v8::Undefined(isolate));
        return;
      }
      auto features = getUnderlying<models::physics::fluid::Features>(info[0]);
      auto underlying = self(info)->create(features);
      auto actor = Actor::Prototype::instance->objectTemplate->NewInstance();
      actor->SetInternalField(0, v8::External::New(isolate, underlying));
      info.GetReturnValue().Set(actor);
    }
  }
}

namespace alcube::scripting::utils {
  using namespace mappings::Actor;
  template <>
  glm::vec3 Accessor<models::Actor, glm::vec3, fields::position>::get(models::Actor* actor) {
    return actor->getPosition();
  }

  template <>
  void Accessor<models::Actor, glm::vec3, fields::position>::set(models::Actor * actor, glm::vec3 v) {
    actor->setPosition(v);
  }

  template <>
  glm::quat Accessor<models::Actor, glm::quat, fields::rotation>::get(models::Actor* actor) {
    return actor->getRotation();
  }

  template <>
  void Accessor<models::Actor, glm::quat, fields::rotation>::set(models::Actor * actor, glm::quat v) {
    actor->setRotation(v);
  }

  template <>
  glm::vec3 Accessor<models::Actor, glm::vec3, fields::linearMomentum>::get(models::Actor* actor) {
    return actor->getLinearMomentum();
  }

  template <>
  void Accessor<models::Actor, glm::vec3, fields::linearMomentum>::set(models::Actor * actor, glm::vec3 v) {
    actor->setLinearMomentum(v);
  }

  template <>
  glm::vec3 Accessor<models::Actor, glm::vec3, fields::angularMomentum>::get(models::Actor* actor) {
    return actor->getAngularMomentum();
  }

  template <>
  void Accessor<models::Actor, glm::vec3, fields::angularMomentum>::set(models::Actor * actor, glm::vec3 v) {
    actor->setAngularMomentum(v);
  }
}
