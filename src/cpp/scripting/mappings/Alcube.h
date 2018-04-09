#ifndef ALCUBE_SCRIPTING_MAPPINGS_ALCUBE_H
#define ALCUBE_SCRIPTING_MAPPINGS_ALCUBE_H

#include "../utils.h"
#include "../../models/Alcube.h"

namespace alcube::scripting::mappings {
  namespace Alcube {
    namespace fields {
      class actorCount {};
    }

    class Prototype :public utils::SingletonPrototype<models::Alcube> {
      public:
        explicit Prototype(models::Alcube* underlying);
        void init() override;
        static Prototype* instance;
        static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void addActor(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void addSpring(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  }
}


#endif //ALCUBE_SCRIPTING_MAPPINGS_ALCUBE_H
