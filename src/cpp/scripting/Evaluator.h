#ifndef ALCUBE_EVALUATOR_H
#define ALCUBE_EVALUATOR_H

#include <vector>

namespace alcube::scripting {
  class Actor {
      unsigned int id;
  };
  class Evaluator {
    private:
      std::vector<Actor> actors;
  };
}


#endif //ALCUBE_EVALUATOR_H
