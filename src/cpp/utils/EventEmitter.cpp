#include "EventEmitter.h"

namespace alcube::utils {
  void EventEmitter::subscribe(alcube::utils::EventHandler *handler) {
    handlers.emplace(handler);
  }

  void EventEmitter::unsubscribe(alcube::utils::EventHandler *handler) {
    handlers.erase(handler);
  }

  void EventEmitter::emit() {
    for (auto handler: handlers) {
      handler->f();
    }
  }

  void EventEmitter::unbind() {
    handlers.clear();
  }
}
