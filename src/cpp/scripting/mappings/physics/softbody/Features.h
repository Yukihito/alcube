#ifndef ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_FEATURES_H
#define ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_FEATURES_H

#include "../../../utils.h"
#include "../../../../models/physics/softbody/Features.h"

namespace alcube::scripting::mappings::physics::softbody {
  namespace Features {
    namespace fields {
      class elasticity {};
      class mass {};
    }

    class Prototype : public utils::TypedPrototype<models::physics::softbody::Features> {
      public:
        explicit Prototype();
        void init() override;
        static Prototype* instance;
    };
  }

  namespace FeaturesFactory {
    class Prototype : public utils::SingletonPrototype<models::physics::softbody::FeaturesFactory> {
      public:
        explicit Prototype(models::physics::softbody::FeaturesFactory* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void create(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_FEATURES_H
