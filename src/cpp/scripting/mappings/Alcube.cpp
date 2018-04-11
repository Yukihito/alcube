#include "Alcube.h"

namespace alcube::scripting::mappings {
  using namespace alcube::scripting::utils;

  namespace Alcube {
    Prototype::Prototype(models::Alcube *underlying) : SingletonPrototype(underlying) {}

    void Prototype::init() {
      utils::Prototype::init();
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      Accessor<models::Alcube, int, fields::actorCount>::define(this);
      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "addActor"),
        v8::FunctionTemplate::New(isolate, Prototype::addActor)
      );
      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "addSpring"),
        v8::FunctionTemplate::New(isolate, Prototype::addSpring)
      );
    }

    void Prototype::addActor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      if (info.Length() < 1) {
        info.GetReturnValue().Set(v8::Undefined(isolate));
        return;
      }
      auto actor = getUnderlying<models::Actor>(info[0]);
      self(info)->add(actor);
      info.GetReturnValue().Set(v8::Undefined(isolate));
    }

    void Prototype::addSpring(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      if (info.Length() < 1) {
        info.GetReturnValue().Set(v8::Undefined(isolate));
        return;
      }
      auto spring = getUnderlying<models::physics::softbody::Spring>(info[0]);
      self(info)->add(spring);
      info.GetReturnValue().Set(v8::Undefined(isolate));
    }
  }
}

namespace alcube::scripting::utils {
  using namespace mappings::Alcube;
  template <>
  int Accessor<models::Alcube, int, fields::actorCount>::get(alcube::models::Alcube* alcube) {
    return alcube->getActorCount();
  }

  template <>
  void Accessor<models::Alcube, int, fields::actorCount>::set(alcube::models::Alcube* alcube, int v) {}
}
