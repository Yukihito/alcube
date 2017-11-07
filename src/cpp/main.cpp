#include "drawing/samples/SimpleSphereSample.h"

int main(int argc, char * argv[]) {
  auto sample = new alcube::drawing::samples::SimpleSphereSample();
  sample->runApp(argc, argv);
  return 0;
}