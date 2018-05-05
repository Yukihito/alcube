#ifndef ALCUBE_SUBSIMULATOR_H
#define ALCUBE_SUBSIMULATOR_H


#include "../gpu/GPUAccessor.h"
#include "Actor.h"

namespace alcube::physics {
  class SubSimulator {
    public:
      virtual void setUpConstants() = 0;
      virtual void input() = 0;
      virtual void updateForce() = 0;
      virtual void motion() = 0;
  };
}

#endif //ALCUBE_SUBSIMULATOR_H
