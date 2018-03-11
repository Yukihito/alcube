#include "SampleApplication.h"

int main(int argc, char * argv[]) {
  auto sample = new alcube::samples::SampleApplication();
  sample->runApp(argc, argv);
  return 0;
}
