#ifndef ALCUBE_SIMULATOR_H
#define ALCUBE_SIMULATOR_H

#include "actor/Actor.h"
#include "link/Link.h"

namespace alcube::alcube {
  class Simulator {
    public:
      virtual void add(actor::Entity* actor) = 0;
      virtual void add(link::Entity* link) = 0;
  };
}

#endif //ALCUBE_SIMULATOR_H
