#include "Features.h"

namespace alcube::scripting::mappings::physics::softbody {
  using namespace alcube::scripting::utils;
  namespace Features {
    void Prototype::init() {
      utils::Prototype::init();
      Accessor<models::physics::softbody::Features, float, fields::elasticity>::define(this);
      Accessor<models::physics::softbody::Features, float, fields::mass>::define(this);
    }
  }

  namespace FeaturesFactory {
    Prototype::Prototype(alcube::models::physics::softbody::FeaturesFactory *underlying) : SingletonPrototype<alcube::models::physics::softbody::FeaturesFactory>(underlying){}

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
      auto underlying = self(info)->create();
      auto features = Features::Prototype::instance->objectTemplate->NewInstance();
      features->SetInternalField(0, v8::External::New(isolate, underlying));
      info.GetReturnValue().Set(features);
    }
  }
}

namespace alcube::scripting::utils {
  using namespace mappings::physics::softbody::Features;

  template <>
  float Accessor<models::physics::softbody::Features, float, fields::mass>::get(models::physics::softbody::Features* features) {
    return features->getMass();
  }

  template <>
  void Accessor<models::physics::softbody::Features, float, fields::mass>::set(models::physics::softbody::Features* features, float v) {
    features->setMass(v);
  };

  template <>
  float Accessor<models::physics::softbody::Features, float, fields::elasticity>::get(models::physics::softbody::Features* features) {
    return features->getElasticity();
  }

  template <>
  void Accessor<models::physics::softbody::Features, float, fields::elasticity>::set(models::physics::softbody::Features* features, float v) {
    features->setElasticity(v);
  };
}
