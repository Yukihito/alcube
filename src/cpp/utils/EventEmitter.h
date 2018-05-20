#ifndef ALCUBE_EVENTEMITTER_H
#define ALCUBE_EVENTEMITTER_H

#include <unordered_set>
#include <string>

namespace alcube::utils {
  template <class T>
  class EventHandler {
    public:
      std::function<void(T&)> f = {};
  };

  template <class T>
  class EventEmitter {
    public:
      void subscribe(EventHandler<T>& handler);
      void unsubscribe(EventHandler<T>& handler);
      void emit(T& event);
      void unbind();
    private:
      std::unordered_set<EventHandler<T>*> handlers = {};
  };

  template <class T>
  void EventEmitter<T>::subscribe(alcube::utils::EventHandler<T> &handler) {
    handlers.emplace(&handler);
  }

  template <class T>
  void EventEmitter<T>::unsubscribe(alcube::utils::EventHandler<T>& handler) {
    handlers.erase(&handler);
  }

  template <class T>
  void EventEmitter<T>::emit(T& event) {
    for (auto handler: handlers) {
      handler->f(event);
    }
  }

  template <class T>
  void EventEmitter<T>::unbind() {
    handlers.clear();
  }
}

#endif //ALCUBE_EVENTEMITTER_H
