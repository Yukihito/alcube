#include "Application.h"

int main(int argc, char * argv[]) {
  auto sample = new alcube::samples::Application(argv[0]);
  sample->run("../src/js/settings/standard.js", "../src/js/samples/fluid-and-softbody.js");
  // sample->run("../src/js/settings/standard.js", "../src/js/samples/softbody.js");
  // sample->run("../src/js/settings/standard.js", "../src/js/samples/fluid.js");
  // sample->run("../src/js/settings/zero-g.js", "../src/js/samples/rigid-bodies.js");
  // sample->run("../src/js/settings/half-size.js", "../src/js/samples/large-amount-of-rigid-bodies.js");
  // sample->run("../src/js/settings/large-size-zero-g.js", "../src/js/samples/stress-test.js");
  return 0;
}
