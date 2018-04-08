#include "Features.h"

namespace alcube::models::physics {
  void Features::init(int id) {
    this->id = id;
  }

  int Features::getId() {
    return id;
  }

  Features* FeaturesFinder::find(int id) {
    for (utils::InstanceFinder<Features>* finder : finders) {
      auto instance = finder->find(id);
      if (instance != nullptr) {
        return instance;
      }
    }
    return nullptr;
  }
}