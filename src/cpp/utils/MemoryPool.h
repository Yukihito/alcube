#ifndef ALCUBE_MEMORYPOOL_H
#define ALCUBE_MEMORYPOOL_H

#include <stack>
#include <map>

namespace alcube::utils {
  template <class T>
  class InstanceFinder {
    public:
      virtual T* find(int id) = 0;
  };

  template <class T>
  class MemoryPool : public InstanceFinder<T> {
    public:
      explicit MemoryPool(int size);
      T* find(int id) override;
      T* get();

    private:
      int instanceCount;
      std::map<int, T*> idToInstance;
      int size;
      T* instances;
      std::stack<int> freeIndices;
  };

  template <class T>
  MemoryPool<T>::MemoryPool(int size) {
    this->size = size;
    instances = new T[size];
    freeIndices = {};
    for (int i = 0; i < size; i++) {
      freeIndices.push(i);
    }
    this->idToInstance = {};
    this->instanceCount = 0;
  }

  template <class T>
  T* MemoryPool<T>::get() {
    auto instance = &instances[freeIndices.top()];
    int nextId = instanceCount;
    instance->init(nextId);
    idToInstance[nextId] = instance;
    freeIndices.pop();
    instanceCount++;
    return instance;
  }

  template <class T>
  T* MemoryPool<T>::find(int id) {
    return idToInstance[id];
  }
}


#endif //ALCUBE_MEMORYPOOL_H
