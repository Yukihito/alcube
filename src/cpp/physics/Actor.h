#ifndef ALCUBE_ACTOR_H
#define ALCUBE_ACTOR_H

#include "../gpu/dtos.h"

namespace alcube::physics {
  enum PhysicalFeature {
    SOFT_BODY,
    FLUID
  };

  class Actor {
    public:
      unsigned short index;
      unsigned short subPhysicalQuantityIndex;
      PhysicalFeature feature;
      gpu::dtos::PhysicalQuantity physicalQuantities;
  };
}


#endif //ALCUBE_ACTOR_H
