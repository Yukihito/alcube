#ifndef ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_SPRING_H
#define ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_SPRING_H

#include "../../../utils.h"
#include "../../../../models/physics/softbody/Spring.h"

namespace alcube::scripting::mappings::physics::softbody {
  class Spring : public utils::TypedPrototype<models::physics::softbody::Spring> {};

  class SpringFactory  : public utils::SingletonPrototype<models::physics::softbody::SpringFactory> {
    public:
      DEFACCESSOR(models::physics::softbody::SpringFactory, float, K);
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>& info);
  };
}
#endif //ALCUBE_SCRIPTING_MAPPINGS_SOFTBODY_SPRING_H
