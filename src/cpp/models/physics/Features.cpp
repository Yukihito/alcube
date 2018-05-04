#include "Features.h"

namespace alcube::models::physics {
  void Features::init(int id, alcube::physics::ActorFactory* actorFactory) {
    this->id = id;
    this->actorFactory = actorFactory;
  }

  int Features::getId() {
    return id;
  }
}