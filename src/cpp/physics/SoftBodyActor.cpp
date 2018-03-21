#include "SoftBodyActor.h"

namespace alcube::physics {
  SoftBodyActor::SoftBodyActor() : Actor() {
    this->actor.type = SOFT_BODY;
    this->subPhysicalQuantity = {};
    this->subPhysicalQuantity.elasticity = 1.0f;
  }
}
