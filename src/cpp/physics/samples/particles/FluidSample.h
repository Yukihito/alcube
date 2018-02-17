#ifndef ALCUBE_FLUIDSAMPLE_H
#define ALCUBE_FLUIDSAMPLE_H

#include "../ApplicationBase.h"
#include "Particles.h"

namespace alcube::physics::samples::particles {
  class FluidSample : public ApplicationBase {
    public:
      explicit FluidSample();
    protected:
      void onInit() override;

      void onDraw() override;
    private:
      std::vector<SoftBodyParticle*> cells;
      Particles* particles;
  };
}

#endif //ALCUBE_FLUIDSAMPLE_H
