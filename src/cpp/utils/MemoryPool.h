#ifndef ALCUBE_MEMORYPOOL_H
#define ALCUBE_MEMORYPOOL_H

#include <stack>
#include <iostream>
#include <unordered_map>

namespace alcube::utils {
  template <class T>
  class MemoryPool {
    public:
      explicit MemoryPool(int size);
      T* allocate();
      void deallocate(T* ptr);

    private:
      int size;
      T* instances;
      std::stack<int> freeIndices;
      std::unordered_map<T*, int> ptrToIndex;
  };

  template <class T>
  MemoryPool<T>::MemoryPool(int size) {
    this->size = size;
    instances = new T[size];
    freeIndices = {};
    ptrToIndex = {};
    for (int i = 0; i < size; i++) {
      freeIndices.push(i);
    }
  }

  template <class T>
  T* MemoryPool<T>::allocate() {
    int index = freeIndices.top();
    auto instance = &instances[index];
    ptrToIndex[instance] = index;
    freeIndices.pop();
    return instance;
  }

  template <class T>
  void MemoryPool<T>::deallocate(T *ptr) {
    freeIndices.push(ptrToIndex[ptr]);
  }
}


#endif //ALCUBE_MEMORYPOOL_H
