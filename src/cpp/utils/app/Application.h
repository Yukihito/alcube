#ifndef ALCUBE_APPLICATION_H
#define ALCUBE_APPLICATION_H

namespace alcube::utils::app {
  class Application {
    public:
      virtual void runApp(int argc, char *argv[]) = 0;
  };
}

#endif //ALCUBE_APPLICATION_H
