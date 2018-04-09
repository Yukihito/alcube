#include "Features.h"

namespace alcube::scripting::mappings::physics::fluid {
  using namespace alcube::scripting::utils;
  namespace Features {
    Prototype* Prototype::instance;
    Prototype::Prototype() {
      Prototype::instance = this;
    }

    void Prototype::init() {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      objectTemplate = v8::ObjectTemplate::New(isolate);
      objectTemplate->SetInternalFieldCount(1);
      Accessor<models::physics::fluid::Features, float, fields::mass>::define(this);
      Accessor<models::physics::fluid::Features, float, fields::density>::define(this);
      Accessor<models::physics::fluid::Features, float, fields::stiffness>::define(this);
      Accessor<models::physics::fluid::Features, float, fields::viscosity>::define(this);
    }
  }

  namespace FeaturesFactory {
    Prototype* Prototype::instance;
    Prototype::Prototype(alcube::models::physics::fluid::FeaturesFactory* underlying) : utils::SingletonPrototype<models::physics::fluid::FeaturesFactory>(underlying) {
      Prototype::instance = this;
    }

    void Prototype::init() {
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      objectTemplate = v8::ObjectTemplate::New(isolate);
      objectTemplate->SetInternalFieldCount(1);
      objectTemplate->Set(
        v8::String::NewFromUtf8(isolate, "create"),
        v8::FunctionTemplate::New(isolate, Prototype::create)
      );
    }

    void Prototype::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate *isolate = v8::Isolate::GetCurrent();
      auto v8Instance = Prototype::instance->objectTemplate->NewInstance();
      v8Instance->SetInternalField(0, v8::External::New(isolate, Prototype::instance->underlying));
      info.GetReturnValue().Set(v8Instance);
    }

    void Prototype::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      v8::HandleScope scope(isolate);
      auto underlying = self(info)->create();
      auto features = Features::Prototype::instance->objectTemplate->NewInstance();
      features->SetInternalField(0, v8::External::New(isolate, underlying));
      info.GetReturnValue().Set(features);
    }
  }
}

namespace alcube::scripting::utils {
  using namespace mappings::physics::fluid::Features;

  template <>
  float Accessor<models::physics::fluid::Features, float, fields::mass>::get(models::physics::fluid::Features* features) {
    return features->getMass();
  }

  template <>
  void Accessor<models::physics::fluid::Features, float, fields::mass>::set(models::physics::fluid::Features* features, float v) {};

  template <>
  float Accessor<models::physics::fluid::Features, float, fields::density>::get(models::physics::fluid::Features* features) {
    return features->getDensity();
  }

  template <>
  void Accessor<models::physics::fluid::Features, float, fields::density>::set(models::physics::fluid::Features* features, float v) {
    features->setDensity(v);
  };

  template <>
  float Accessor<models::physics::fluid::Features, float, fields::stiffness>::get(models::physics::fluid::Features* features) {
    return features->getStiffness();
  }

  template <>
  void Accessor<models::physics::fluid::Features, float, fields::stiffness>::set(models::physics::fluid::Features* features, float v) {
    features->setStiffness(v);
  };

  template <>
  float Accessor<models::physics::fluid::Features, float, fields::viscosity>::get(models::physics::fluid::Features* features) {
    return features->getViscosity();
  }

  template <>
  void Accessor<models::physics::fluid::Features, float, fields::viscosity>::set(models::physics::fluid::Features* features, float v) {
    features->setViscosity(v);
  };
}
