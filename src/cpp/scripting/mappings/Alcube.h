#ifndef ALCUBE_SCRIPTING_MAPPINGS_ALCUBE_H
#define ALCUBE_SCRIPTING_MAPPINGS_ALCUBE_H

#include "../utils.h"
#include "../../models/Alcube.h"

namespace alcube::scripting::mappings {
  class Alcube : public utils::SingletonPrototype<models::Alcube> {
    public:
      DEFGETTER(models::Alcube, int, ActorCount);
      void init() override;
  };
}


#endif //ALCUBE_SCRIPTING_MAPPINGS_ALCUBE_H
