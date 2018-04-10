#include "SampleApplication.h"

int main(int argc, char * argv[]) {
  auto settings = new alcube::models::Settings();
  settings->physics.gravity *= 2.0f;
  auto sample = new alcube::samples::SampleApplication(settings, argv[0]);
  sample->runApp(argc, argv);
  return 0;
}
