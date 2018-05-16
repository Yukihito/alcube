#include "EventEmitter.h"

namespace alcube::utils {
  void EventEmitter::subscribe(std::function<void()> f) {
    callbacks.push_back(f);
  }

  void EventEmitter::emit() {
    for (auto f : callbacks) {
      f();
    }
  }

  void EventEmitter::unbind() {
    callbacks.clear();
  }
}
