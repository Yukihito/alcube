#ifndef ALCUBE_PARTICLESSAMPLE_H
#define ALCUBE_PARTICLESSAMPLE_H

#include "../BaseApplication.h"
#include "Particles.h"

namespace alcube::physics::samples::particles {
  class ParticlesSample : public BaseApplication {
    public:
      explicit ParticlesSample();
    protected:
      void onInit() override;

      void onDraw() override;
    private:
      std::vector<Cell*> cells;
      Particles* particles;
  };
}

#endif //ALCUBE_PARTICLESSAMPLE_H
