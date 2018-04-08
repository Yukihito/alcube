#include "Prototype.h"

namespace alcube::scripting::mappings {
  using namespace alcube::scripting::utils;
  namespace Actor {
    Prototype *Prototype::instance;

    Prototype::Prototype() {
      Prototype::instance = this;
    }

    void Prototype::init() {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      objectTemplate = v8::ObjectTemplate::New(isolate);
      objectTemplate->SetInternalFieldCount(1);
      Accessor<models::Actor, glm::vec3, variables::position>::create(this);
      Accessor<models::Actor, glm::quat, variables::rotation>::create(this);
      Accessor<models::Actor, glm::vec3, variables::linearMomentum>::create(this);
      Accessor<models::Actor, glm::vec3, variables::angularMomentum>::create(this);
    }
  }

  namespace ActorFactory {
    Prototype *Prototype::instance;

    Prototype::Prototype(models::ActorFactory *underlying) {
      this->underlying = underlying;
      Prototype::instance = this;
    }

    void Prototype::init() {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      objectTemplate = v8::ObjectTemplate::New(isolate);
      objectTemplate->SetInternalFieldCount(1);

      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "createFluid"),
        v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value> &args) {
          v8::Local<v8::Object> self = args.Holder();
          v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
          void *ptr = wrap->Value();
          auto instance = static_cast<models::ActorFactory *>(ptr);
          v8::Isolate *isolate = v8::Isolate::GetCurrent();
          v8::HandleScope scope(isolate);
          if (args.Length() < 1) {
            args.GetReturnValue().Set(v8::Undefined(isolate));
          }
          v8::Local<v8::Object> featuresDTO = args[0]->ToObject();
          auto density = (float) featuresDTO->Get(v8str(isolate, "density"))->NumberValue();
          auto stiffness = (float) featuresDTO->Get(v8str(isolate, "stiffness"))->NumberValue();
          auto viscosity = (float) featuresDTO->Get(v8str(isolate, "viscosity"))->NumberValue();
          models::physics::fluid::Features features = models::physics::fluid::Features();
          features.setDensity(density);
          features.setStiffness(stiffness);
          features.setViscosity(viscosity);
          auto underlying = instance->create(&features);
          auto actor = Actor::Prototype::instance->objectTemplate->NewInstance();
          actor->SetInternalField(0, v8::External::New(isolate, underlying));
          args.GetReturnValue().Set(actor);
        }));
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &args) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto actorFactory = Prototype::instance->objectTemplate->NewInstance();
      actorFactory->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      args.GetReturnValue().Set(actorFactory);
    }
  }
}

namespace alcube::scripting::utils {
  using namespace mappings::Actor;
  template <>
  glm::vec3 Accessor<models::Actor, glm::vec3, variables::position>::get(models::Actor* actor) {
    return actor->getPosition();
  }

  template <>
  void Accessor<models::Actor, glm::vec3, variables::position>::set(models::Actor * actor, glm::vec3 v) {
    actor->setPosition(v);
  }

  template <>
  glm::quat Accessor<models::Actor, glm::quat, variables::rotation>::get(models::Actor* actor) {
    return actor->getRotation();
  }

  template <>
  void Accessor<models::Actor, glm::quat, variables::rotation>::set(models::Actor * actor, glm::quat v) {
    actor->setRotation(v);
  }

  template <>
  glm::vec3 Accessor<models::Actor, glm::vec3, variables::linearMomentum>::get(models::Actor* actor) {
    return actor->getLinearMomentum();
  }

  template <>
  void Accessor<models::Actor, glm::vec3, variables::linearMomentum>::set(models::Actor * actor, glm::vec3 v) {
    actor->setLinearMomentum(v);
  }

  template <>
  glm::vec3 Accessor<models::Actor, glm::vec3, variables::angularMomentum>::get(models::Actor* actor) {
    return actor->getAngularMomentum();
  }

  template <>
  void Accessor<models::Actor, glm::vec3, variables::angularMomentum>::set(models::Actor * actor, glm::vec3 v) {
    actor->setAngularMomentum(v);
  }
}
