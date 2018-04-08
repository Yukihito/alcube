#include "Actor.h"

namespace alcube::scripting::mappings {
  using namespace utils;
  namespace Actor {
    Prototype *Prototype::instance;

    Prototype::Prototype() {
      Prototype::instance = this;
    }

    void Prototype::init() {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      objectTemplate = v8::ObjectTemplate::New(isolate);
      objectTemplate->SetInternalFieldCount(1);
      utils::Accessor<models::Actor, glm::vec3, fields::position>::create(this);
      utils::Accessor<models::Actor, glm::quat, fields::rotation>::create(this);
      utils::Accessor<models::Actor, glm::vec3, fields::linearMomentum>::create(this);
      utils::Accessor<models::Actor, glm::vec3, fields::angularMomentum>::create(this);
    }
  }

  namespace ActorFactory {
    Prototype *Prototype::instance;

    Prototype::Prototype(models::ActorFactory *underlying) : SingletonPrototype<models::ActorFactory>(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      objectTemplate = v8::ObjectTemplate::New(isolate);
      objectTemplate->SetInternalFieldCount(1);
      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "createFluid"),
        v8::FunctionTemplate::New(isolate, Prototype::createFluid));
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto actorFactory = Prototype::instance->objectTemplate->NewInstance();
      actorFactory->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(actorFactory);
    }

    void Prototype::createFluid(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      v8::HandleScope scope(isolate);
      if (info.Length() < 1) {
        info.GetReturnValue().Set(v8::Undefined(isolate));
      }
      v8::Local<v8::Object> featuresDTO = info[0]->ToObject();
      auto density = (float) featuresDTO->Get(utils::v8str(isolate, "density"))->NumberValue();
      auto stiffness = (float) featuresDTO->Get(utils::v8str(isolate, "stiffness"))->NumberValue();
      auto viscosity = (float) featuresDTO->Get(utils::v8str(isolate, "viscosity"))->NumberValue();
      models::physics::fluid::Features features = models::physics::fluid::Features();
      features.setDensity(density);
      features.setStiffness(stiffness);
      features.setViscosity(viscosity);
      auto underlying = self(info)->create(&features);
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
