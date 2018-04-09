#include "Features.h"

namespace alcube::models::physics {
  void Features::init(int id) {
    this->id = id;
  }

  int Features::getId() {
    return id;
  }
}