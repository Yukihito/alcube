#ifndef ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H
#define ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H

#include "v8.h"
#include "../../../../models/physics/fluid/Features.h"
#include "../../../utils.h"

namespace alcube::scripting::mappings::physics::fluid {
  class Features : public utils::TypedPrototype<models::physics::fluid::Features> {
    public:
      void init() override;
      DEFGETTER(models::physics::fluid::Features, float, Mass);
      DEFACCESSOR(models::physics::fluid::Features, float, Density);
      DEFACCESSOR(models::physics::fluid::Features, float, Stiffness);
      DEFACCESSOR(models::physics::fluid::Features, float, Viscosity);
  };

  class FeaturesFactory : public utils::SingletonPrototype<models::physics::fluid::FeaturesFactory> {
    public:
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>& info);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H
