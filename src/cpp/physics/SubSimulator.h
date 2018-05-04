#ifndef ALCUBE_SUBSIMULATOR_H
#define ALCUBE_SUBSIMULATOR_H


#include "../gpu/GPUAccessor.h"
#include "Actor.h"

namespace alcube::physics {
  class SubSimulator {
    public:
      //virtual void set(gpu::GPUAccessor* gpuAccessor) = 0;
      virtual void setUpConstants() = 0;
      virtual void setUpComputingSize() = 0;
      virtual void writeHostMemories() = 0;
      virtual void setUpMemories() = 0;
      virtual void updateForce() = 0;
      virtual void motion() = 0;
      virtual bool add(Actor* actor) = 0;
  };
}

#endif //ALCUBE_SUBSIMULATOR_H
