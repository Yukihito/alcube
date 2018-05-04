#include "ActorResources.h"

namespace alcube::physics {
  ActorResources::ActorResources(unsigned int size) {
    allocationRange = new utils::AllocationRange(0, size);
    fluidResource = new ActorResource<fluid::Actor>(size);
    softbodyResource = new ActorResource<softbody::Actor>(size);
  }
}