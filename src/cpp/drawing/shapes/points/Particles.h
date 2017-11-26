#ifndef ALCUBE_PARTICLES_H
#define ALCUBE_PARTICLES_H

#include "../../Shape.h"

namespace alcube::drawing::shapes::points {
  class Particles : public Shape {
    public:
      void draw() override;
      explicit Particles(unsigned int maxParticleCount);
      unsigned int particleCount;

    protected:
      Buffer* createBuffer() override;

    private:
      unsigned int maxParticleCount;
  };
}

#endif //ALCUBE_PARTICLES_H
