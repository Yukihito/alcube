#include "drawing/samples/SimpleSphereSample.h"
#include "physics/samples/BenchmarkSample.h"
#include <iostream>


int main(int argc, char * argv[]) {
  //auto sample = new alcube::drawing::samples::SimpleSphereSample();
  auto sample = new alcube::physics::samples::BenchmarkSample();
  sample->runApp(argc, argv);
  return 0;
}