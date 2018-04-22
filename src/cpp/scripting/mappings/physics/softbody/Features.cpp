#include "Features.h"

namespace alcube::scripting::mappings::physics::softbody {
  using namespace utils;
  void Features::init() {
    Prototype::init();
    define<float, Elasticity>();
    define<float, Mass>();
  }

  void FeaturesFactory::init() {
    Prototype::init();
    DEFMETHOD(create);
    classNamePrefix = "Softbody";
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
