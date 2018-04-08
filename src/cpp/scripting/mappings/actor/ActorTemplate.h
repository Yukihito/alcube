#ifndef ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
#define ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H

#include "v8.h"
#include "../../../models/actor/Actor.h"
#include "../../utils.h"
#include "../../../models/physics/fluid/Features.h"

namespace alcube::scripting::mappings::actor {
  namespace variables {
    class position {};
    class rotation {};
    class linearMomentum {};
    class angularMomentum {};
  }

  class ActorTemplate : public utils::Template {
    public:
      explicit ActorTemplate();
      void init() override;
      static ActorTemplate* instance;
  };

  class ActorFactoryTemplate : public utils::Template {
    public:
      explicit ActorFactoryTemplate(models::actor::ActorFactory* underlying);
      void init() override;
      static ActorFactoryTemplate* instance;
      static void constructor(const v8::FunctionCallbackInfo<v8::Value>&);
    private:
      alcube::models::actor::ActorFactory* underlying;
  };
}

#endif //ALCUBE_SCRIPTING_MAPPINGS_ACTOR_H
