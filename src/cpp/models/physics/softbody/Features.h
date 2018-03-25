#ifndef ALCUBE_SOFTBODY_FEATURES_H
#define ALCUBE_SOFTBODY_FEATURES_H

#include "../../../physics/softbody/Actor.h"
#include "../Features.h"

namespace alcube::models::physics::softbody {
  class Features : public models::physics::Features {
    public:
      float getElasticity();
      void setElasticity(float arg);
      float getMass();
      void setMass(float arg);
      alcube::physics::Actor* createPhysicsActor() override;

    private:
      float elasticity = 1.0f;
      float mass = 1.0f;
  };
}

#endif //ALCUBE_SOFTBODY_FEATURES_H
