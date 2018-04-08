#ifndef ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H
#define ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H

#include "v8.h"
#include "../../../../models/physics/fluid/Features.h"
#include "../../../utils.h"

namespace alcube::scripting::mappings::physics::fluid {
  class Features {
    public:
      static void initObjectTemplate();

    private:
      static void getMass(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
      static void getDensity(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
      static void setDensity(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
      static void getStiffness(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
      static void setStiffness(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
      static void getViscosity(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
      static void setViscosity(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
  };

  class FeaturesFactory {
    public:
      static void create(const v8::FunctionCallbackInfo<v8::Value> &args);
      static models::physics::fluid::FeaturesFactory* instance;
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H
