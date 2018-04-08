#ifndef ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
#define ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H

#include "v8.h"
#include "../../models/Actor.h"
#include "../utils.h"
#include "../../models/physics/fluid/Features.h"

namespace alcube::scripting::mappings {
  namespace Actor {
    namespace fields {
      class position {};
      class rotation {};
      class linearMomentum {};
      class angularMomentum {};
    }

    class Prototype : public utils::TypedPrototype<models::Actor> {
      public:
        explicit Prototype();
        void init() override;
        static Prototype* instance;
    };
  }

  namespace ActorFactory {
    class Prototype : public utils::SingletonPrototype<models::ActorFactory> {
      public:
        explicit Prototype(models::ActorFactory* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>&);
        static void createFluid(const v8::FunctionCallbackInfo<v8::Value>&);
    };
  }
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
