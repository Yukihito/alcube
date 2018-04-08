#ifndef ALCUBE_SOFTBODY_FEATURES_H
#define ALCUBE_SOFTBODY_FEATURES_H

#include "../../../physics/softbody/Actor.h"
#include "../Features.h"
#include "../../../utils/MemoryPool.h"

namespace alcube::models::physics::softbody {
  class Features : public models::physics::Features {
    public:
      float getElasticity();
      void setElasticity(float arg);
      float getMass();
      void setMass(float arg);
      alcube::physics::Actor* createPhysicsActor() override;
      void init(int id) override;

    private:
      float elasticity = 1.0f;
      float mass = 1.0f;
  };

  class FeaturesFactory {
    public:
      explicit FeaturesFactory(utils::MemoryPool<Features>* memoryPool);
      Features* create();

    private:
      utils::MemoryPool<Features>* memoryPool;
  };
}

#endif //ALCUBE_SOFTBODY_FEATURES_H
