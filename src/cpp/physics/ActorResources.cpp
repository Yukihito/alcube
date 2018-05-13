#include "ActorResources.h"

namespace alcube::physics {
  ActorResources::ActorResources(unsigned int size) {
    allocationRangeMemoryPool = new utils::MemoryPool<utils::AllocationRange>(size * 8);
    allocationRange = allocationRangeMemoryPool->get();
    allocationRange->init(0, size, allocationRangeMemoryPool);
    fluidResource = new ActorResource<fluid::Actor>(size, allocationRangeMemoryPool);
    softbodyResource = new ActorResource<softbody::Actor>(size, allocationRangeMemoryPool);
    springResource = new ActorResource<softbody::Spring>(size * 4, allocationRangeMemoryPool);
    entities = new Actor*[size];
  }
}
