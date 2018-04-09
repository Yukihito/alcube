#include "Alcube.h"

namespace alcube::scripting::mappings {
  using namespace alcube::scripting::utils;

  namespace Alcube {
    Prototype* Prototype::instance;
    Prototype::Prototype(models::Alcube *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

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

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto v8Instance = Prototype::instance->objectTemplate->NewInstance();
      v8Instance->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(v8Instance);
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
