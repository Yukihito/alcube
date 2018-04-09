#ifndef ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_SPRING_H
#define ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_SPRING_H

#include "../../../utils.h"
#include "../../../../models/physics/softbody/Spring.h"

namespace alcube::scripting::mappings::physics::softbody {
  namespace Spring {
    class Prototype : public utils::TypedPrototype<models::physics::softbody::Spring> {
      public:
        explicit Prototype();
        static Prototype* instance;
    };
  }

  namespace SpringFactory {
    namespace fields {
      class k {};
    }

    class Prototype : public utils::SingletonPrototype<models::physics::softbody::SpringFactory> {
      public:
        explicit Prototype(models::physics::softbody::SpringFactory* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void create(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }
}
#endif //ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_SPRING_H
