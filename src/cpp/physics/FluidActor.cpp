#include "FluidActor.h"

namespace alcube::physics {
  FluidActor::FluidActor() : Actor() {
    this->actor.type = FLUID;
    this->subPhysicalQuantity = {};
  }
}