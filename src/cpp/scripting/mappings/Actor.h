#ifndef ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
#define ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H

#include "v8.h"
#include "../../models/Actor.h"
#include "../utils.h"
#include "../../models/physics/fluid/Features.h"

namespace alcube::scripting::mappings {
  class Actor  : public utils::TypedPrototype<models::Actor> {
    public:
      void init() override;
      DEFPARAM(models::Actor, glm::vec3, position);
      DEFPARAM(models::Actor, glm::quat, rotation);
      DEFPARAM(models::Actor, glm::vec3, linearMomentum);
      DEFPARAM(models::Actor, glm::vec3, angularMomentum);
  };

  class ActorFactory : public utils::SingletonPrototype<models::ActorFactory>  {
    public:
      explicit ActorFactory(models::ActorFactory* underlying);
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>&);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
