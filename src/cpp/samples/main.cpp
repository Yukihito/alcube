#include "ApplicationBase.h"

int main(int argc, char * argv[]) {
  auto sample = new alcube::samples::ApplicationBase(argv[0]);
  sample->run();
  return 0;
}
