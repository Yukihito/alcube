#ifndef ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H
#define ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H

#include "v8.h"
#include "../../../../models/physics/fluid/Features.h"
#include "../../../utils.h"

namespace alcube::scripting::mappings::physics::fluid {
  namespace Features {
    namespace fields {
      class mass {};
      class density {};
      class stiffness {};
      class viscosity {};
    }

    class Prototype : public utils::TypedPrototype<models::physics::fluid::Features> {
      public:
        void init() override;
    };
  }

  namespace FeaturesFactory {
    class Prototype : public utils::SingletonPrototype<models::physics::fluid::FeaturesFactory> {
      public:
        explicit Prototype(models::physics::fluid::FeaturesFactory* underlying);
        void init() override;
        static void create(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_FLUID_FEATURES_H
