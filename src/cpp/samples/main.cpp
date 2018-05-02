#include "Application.h"

int main(int argc, char * argv[]) {
  auto sample = new alcube::samples::Application(argv[0]);
  sample->run("../src/js/settings.js", "../src/js/samples/fluid.js");
  return 0;
}
