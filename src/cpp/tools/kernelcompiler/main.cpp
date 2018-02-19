#include <iostream>

#include "../../utils/opencl/Resources.h"
#include "../../utils/FileUtil.h"
#include "../../utils/opencl/ProgramFactory.h"

int main(int argc, char * argv[]) {
  auto *resources = new alcube::utils::opencl::Resources();
  auto *fileUtil = new alcube::utils::FileUtil();

  auto *programFactory = new alcube::utils::opencl::ProgramFactory(resources, fileUtil);
  programFactory->create("src/kernels/generated-code/all.cl");
  return 0;
}