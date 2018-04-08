#include "Features.h"

namespace alcube::scripting::mappings::physics::fluid {
  using namespace alcube::scripting::utils;

  void Features::initObjectTemplate() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    /*
    Features::objectTemplateHolder->objectTemplate = v8::ObjectTemplate::New(isolate);
    Features::objectTemplateHolder->objectTemplate->SetInternalFieldCount(1);
    Features::objectTemplateHolder->objectTemplate->SetAccessor(v8::String::NewFromUtf8(isolate, "mass"), Features::getMass);
    Features::objectTemplateHolder->objectTemplate->SetAccessor(v8::String::NewFromUtf8(isolate, "density"), Features::getDensity, Features::setDensity);
    Features::objectTemplateHolder->objectTemplate->SetAccessor(v8::String::NewFromUtf8(isolate, "stiffness"), Features::getStiffness, Features::setStiffness);
    Features::objectTemplateHolder->objectTemplate->SetAccessor(v8::String::NewFromUtf8(isolate, "viscosity"), Features::getViscosity, Features::setViscosity);
     */
  }

  /*
  void Features::getMass(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
    vec3Getter<models::physics::fluid::Features>(info, [](models::physics::fluid::Features* features) {
      return features->getMass();
    });
  }
   */
}