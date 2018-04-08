#ifndef ALCUBE_PHYSICSFEATURE_H
#define ALCUBE_PHYSICSFEATURE_H

#include "../../physics/Actor.h"
#include "../../utils/MemoryPool.h"
#include <vector>

namespace alcube::models::physics {
  class Features {
    public:
      int getId();
      virtual alcube::physics::Actor* createPhysicsActor() = 0;
      virtual void init(int id);

    private:
      int id = 0;
  };

  template <class T>
  class SubFeaturesFinderContainer : utils::InstanceFinder<Features> {
    public:
      explicit SubFeaturesFinderContainer(utils::InstanceFinder<T>* base);
      Features* find(int id) override;
    private:
      utils::InstanceFinder<T>* base;
  };

  template <class T>
  SubFeaturesFinderContainer<T>::SubFeaturesFinderContainer(alcube::utils::InstanceFinder<T> *base) {
    this->base = base;
  }

  template <class T>
  Features* SubFeaturesFinderContainer<T>::find(int id) {
    return static_cast<Features*>(base->find(id));
  }

  template <class T>
  class SubFeaturesMemoryPool : public utils::MemoryPool<T> {
    public:
      explicit SubFeaturesMemoryPool(int size);
      utils::InstanceFinder<Features>* upCast();
  };

  template <class T>
  SubFeaturesMemoryPool<T>::SubFeaturesMemoryPool(int size) : utils::MemoryPool<T>(size){}

  template <class T>
  utils::InstanceFinder<Features>* SubFeaturesMemoryPool<T>::upCast() {
    return new SubFeaturesFinderContainer<T>(this);
  }

  class FeaturesFinder : utils::InstanceFinder<Features> {
    public:
      Features* find(int id) override;
      template <class T>
      void addMemoryPool(SubFeaturesMemoryPool<T> *finder);
    private:
      std::vector<utils::InstanceFinder<Features>*> finders = {};
  };

  template <class T>
  void FeaturesFinder::addMemoryPool(alcube::models::physics::SubFeaturesMemoryPool<T> *finder) {
    finders.push_back(finder->upCast());
  }
}

#endif //ALCUBE_PHYSICSFEATURE_H
