#ifndef ALCUBE_LINK_H
#define ALCUBE_LINK_H

#include "../physics/Features.h"

namespace alcube::models::link {
  class Identifier {};

  class Entity  {
    public:
      virtual Identifier getId() = 0;
  };

  class Repository {
    public:
      virtual Entity* find(Identifier id) = 0;
      virtual void store(Entity* entity) = 0;
      virtual void del(Identifier id) = 0;
  };

  class Factory {
    public:
      virtual Entity* create(physics::LinkFeatures* feature) = 0;
  };
}

#endif //ALCUBE_LINK_H
