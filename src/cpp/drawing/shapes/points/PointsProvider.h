#ifndef ALCUBE_POINTSPROVIDER_H
#define ALCUBE_POINTSPROVIDER_H

#include "Sphere.h"
#include "Particles.h"

namespace alcube::drawing::shapes::points {
  class PointsProvider {
    public:
      Sphere sphere;
      Particles* createParticles(unsigned int maxParticleCount);
      explicit PointsProvider();
  };
}

#endif //ALCUBE_POINTSPROVIDER_H
