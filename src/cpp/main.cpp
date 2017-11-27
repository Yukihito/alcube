#include "drawing/samples/SimpleSphereSample.h"
#include "physics/samples/BenchmarkSample.h"
#include "physics/samples/balls/BallsSample.h"
#include "physics/samples/particles/ParticlesSample.h"
#include <iostream>


int main(int argc, char * argv[]) {
  //auto sample = new alcube::drawing::samples::SimpleSphereSample();
  //auto sample = new alcube::physics::samples::BenchmarkSample();
  //auto sample = new alcube::physics::samples::balls::BallsSample();
  auto sample = new alcube::physics::samples::particles::ParticlesSample();
  sample->runApp(argc, argv);
  return 0;
}