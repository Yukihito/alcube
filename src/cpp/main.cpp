#include "drawing/samples/SimpleSphereSample.h"

int main(int argc, char * argv[]) {
  auto sample = new drawing::samples::SimpleSphereSample();
  sample->runApp(argc, argv);
  return 0;
}