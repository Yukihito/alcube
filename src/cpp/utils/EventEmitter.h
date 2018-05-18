#ifndef ALCUBE_EVENTEMITTER_H
#define ALCUBE_EVENTEMITTER_H

#include <unordered_set>
#include <string>

namespace alcube::utils {
  class EventHandler {
    public:
      std::function<void()> f = {};
  };

  class EventEmitter {
    public:
      void subscribe(EventHandler* handler);
      void unsubscribe(EventHandler* handler);
      void emit();
      void unbind();
    private:
      std::unordered_set<EventHandler*> handlers = {};
  };
}

#endif //ALCUBE_EVENTEMITTER_H
