#include "KernelFactory.h"

namespace alcube::utils::opencl {
  KernelFactory::KernelFactory(Resources *resources, FileUtil *fileUtil) {
    this->resources = resources;
  }

  cl_kernel KernelFactory::create(cl_program program, const char *name) {
    cl_int status;
    cl_kernel kernel = clCreateKernel(program, name, &status);
    std::cout << "status clCreateKernel: " << status << std::endl;
    this->resources->kernels.push_back(kernel);
    return kernel;
  }
}
