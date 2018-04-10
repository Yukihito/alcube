#include "Settings.h"

namespace alcube::scripting::mappings {
  using namespace alcube::scripting::utils;
  namespace PhysicsSettings {
    Prototype* Prototype::instance;
    Prototype::Prototype(alcube::models::PhysicsSettings *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::PhysicsSettings, float, fields::timeStepSize>::define(this);
      Accessor<models::PhysicsSettings, float, fields::gravity>::define(this);
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto v8Instance = Prototype::instance->objectTemplate->NewInstance();
      v8Instance->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(v8Instance);
    }
  }

  namespace WindowSettings {
    Prototype* Prototype::instance;
    Prototype::Prototype(alcube::models::WindowSettings *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::WindowSettings, int, fields::width>::define(this);
      Accessor<models::WindowSettings, int, fields::height>::define(this);
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto v8Instance = Prototype::instance->objectTemplate->NewInstance();
      v8Instance->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(v8Instance);
    }
  }

  namespace WorldSettings {
    Prototype* Prototype::instance;
    Prototype::Prototype(alcube::models::WorldSettings *underlying) : SingletonPrototype(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::WorldSettings, float, fields::size>::define(this);
      Accessor<models::WorldSettings, int, fields::maxActorCount>::define(this);
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto v8Instance = Prototype::instance->objectTemplate->NewInstance();
      v8Instance->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(v8Instance);
    }
  }
}
