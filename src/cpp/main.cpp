#include "physics/samples/balls/SoftBodyAndFluidInteraction.h"
#include <iostream>

int main(int argc, char * argv[]) {
  //auto sample = new alcube::drawing::samples::SimpleSphereSample();
  //auto sample = new alcube::physics::samples::BenchmarkSample();
  //auto sample = new alcube::physics::samples::balls::BallsSample();
  //auto sample = new alcube::physics::samples::particles::ParticlesSample();
  //auto sample = new alcube::physics::samples::balls::ManyBallsSample();
  //auto sample = new alcube::physics::samples::balls::SoftBodySample();
  //auto sample = new alcube::physics::samples::particles::FluidSample();
  auto sample = new alcube::physics::samples::balls::SoftBodyAndFluidInteraction();
  sample->runApp(argc, argv);
  return 0;
}