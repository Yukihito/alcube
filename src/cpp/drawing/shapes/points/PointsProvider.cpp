#include "PointsProvider.h"

namespace alcube::drawing::shapes::points {
  PointsProvider::PointsProvider() : sphere(32) {}

  Particles* PointsProvider::createParticles(unsigned int maxParticleCount) {
    auto particles = new Particles(maxParticleCount);
    return particles;
  }
}

