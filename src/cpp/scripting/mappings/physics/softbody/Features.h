#ifndef ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_FEATURES_H
#define ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_FEATURES_H

#include "../../../utils.h"
#include "../../../../models/physics/softbody/Features.h"

namespace alcube::scripting::mappings::physics::softbody {
  class Features : public utils::TypedPrototype<models::physics::softbody::Features> {
    public:
      void init() override;
      DEFACCESSOR(models::physics::softbody::Features, float, Elasticity);
      DEFACCESSOR(models::physics::softbody::Features, float, Mass);
  };

  class FeaturesFactory  : public utils::SingletonPrototype<models::physics::softbody::FeaturesFactory> {
    public:
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>& info);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_FEATURES_H
