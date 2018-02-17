#ifndef ALCUBE_PARTICLESSAMPLE_H
#define ALCUBE_PARTICLESSAMPLE_H

#include "../ApplicationBase.h"
#include "Particles.h"

namespace alcube::physics::samples::particles {
  class ParticlesSample : public ApplicationBase {
    public:
      explicit ParticlesSample();
    protected:
      void onInit() override;

      void onDraw() override;
    private:
      std::vector<SoftBodyParticle*> cells;
      Particles* particles;
  };
}

#endif //ALCUBE_PARTICLESSAMPLE_H
