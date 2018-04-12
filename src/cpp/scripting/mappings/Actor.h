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
      DEFACCESSOR(models::Actor, glm::vec3, Position);
      DEFACCESSOR(models::Actor, glm::quat, Rotation);
      DEFACCESSOR(models::Actor, glm::vec3, LinearMomentum);
      DEFACCESSOR(models::Actor, glm::vec3, AngularMomentum);
  };

  class ActorFactory : public utils::SingletonPrototype<models::ActorFactory>  {
    public:
      explicit ActorFactory(models::ActorFactory* underlying);
      void init() override;
      static void create(const v8::FunctionCallbackInfo<v8::Value>&);
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
