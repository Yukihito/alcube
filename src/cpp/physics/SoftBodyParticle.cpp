#include "SoftBodyParticle.h"

namespace alcube::physics {
  SoftBodyParticle::SoftBodyParticle() : Sphere() {
    alterEgo = nullptr;
    radiusForAlterEgo = 0.0f;
  }
}