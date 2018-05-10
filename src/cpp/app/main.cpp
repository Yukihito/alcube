#include "Application.h"

int main(int argc, char * argv[]) {
  auto app = new alcube::app::Application(argv[0]);
  app->run("../src/js/settings/standard.js", "../src/js/samples/blank.js");
  // app->run("../src/js/settings/standard.js", "../src/js/samples/fluid-and-softbody.js");
  // app->run("../src/js/settings/standard.js", "../src/js/samples/softbody.js");
  // app->run("../src/js/settings/standard.js", "../src/js/samples/fluid.js");
  // app->run("../src/js/settings/zero-g.js", "../src/js/samples/rigid-bodies.js");
  // app->run("../src/js/settings/half-size.js", "../src/js/samples/large-amount-of-rigid-bodies.js");
  // app->run("../src/js/settings/large-size-zero-g.js", "../src/js/samples/stress-test.js");
  return 0;
}
