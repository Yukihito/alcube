#ifndef ALCUBE_EVENTEMITTER_H
#define ALCUBE_EVENTEMITTER_H

#include <unordered_map>
#include <vector>
#include <string>

namespace alcube::utils {
  class EventEmitter {
    public:
      void subscribe(std::function<void()> f);
      void emit();
    private:
      std::vector<std::function<void()>> callbacks = {};
  };
}

#endif //ALCUBE_EVENTEMITTER_H
