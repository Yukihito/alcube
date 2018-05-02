#ifndef ALCUBE_MEMORYPOOL_H
#define ALCUBE_MEMORYPOOL_H

#include <stack>
#include <map>
#include <iostream>

namespace alcube::utils {
  template <class T>
  class MemoryPool {
    public:
      explicit MemoryPool(int size);
      T* get();

    private:
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
  }

  template <class T>
  T* MemoryPool<T>::get() {
    auto instance = &instances[freeIndices.top()];
    freeIndices.pop();
    return instance;
  }
}


#endif //ALCUBE_MEMORYPOOL_H
