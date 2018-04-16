#include "Features.h"

namespace alcube::scripting::mappings::physics::fluid {
  using namespace utils;
  void Features::init() {
      Prototype::init();
      define<float, Mass>();
      define<float, Density>();
      define<float, Stiffness>();
      define<float, Viscosity>();
  }

  //FeaturesFactory::FeaturesFactory() : utils::SingletonPrototype<models::physics::fluid::FeaturesFactory>() {}

  void FeaturesFactory::init() {
    Prototype::init();
    DEFMETHOD(create);
    classNamePrefix = "Fluid";
  }

  void FeaturesFactory::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
      v8::Isolate* isolate = v8::Isolate::GetCurrent();
      v8::HandleScope scope(isolate);
      auto underlying = self(info)->create();
      auto features = Features::instance->objectTemplate->NewInstance();
      features->SetInternalField(0, v8::External::New(isolate, underlying));
      info.GetReturnValue().Set(features);
  }
}
