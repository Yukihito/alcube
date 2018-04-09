#ifndef ALCUBE_FLUID_FEATURES_H
#define ALCUBE_FLUID_FEATURES_H

#include "../../../physics/fluid/Actor.h"
#include "../Features.h"
#include "../../../utils/MemoryPool.h"

namespace alcube::models::physics::fluid {
  class Features : public models::physics::Features {
    public:
      float getMass();
      float getDensity();
      void setDensity(float arg);
      float getStiffness();
      void setStiffness(float arg);
      float getViscosity();
      void setViscosity(float arg);
      alcube::physics::Actor* createPhysicsActor() override;
  };

  class FeaturesFactory {
    public:
      explicit FeaturesFactory(utils::MemoryPool<Features>* memoryPool);
      Features* create();

    private:
      int instanceCount = 0;
      utils::MemoryPool<Features>* memoryPool;
  };
}

#endif //ALCUBE_FLUID_FEATURES_H
