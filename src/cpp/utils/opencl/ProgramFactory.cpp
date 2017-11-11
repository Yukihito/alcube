#include "ProgramFactory.h"

namespace alcube::utils::opencl {
  ProgramFactory::ProgramFactory(Resources *resources, FileUtil *fileUtil) {
    this->resources = resources;
    this->fileUtil = fileUtil;
  }

  cl_program ProgramFactory::create(const char* path) {
    cl_int status;
    static const char *src[] = {fileUtil->readFile(path).c_str()};
    cl_program program = clCreateProgramWithSource(this->resources->context, 1, (const char **) &src, nullptr, nullptr);
    status = clBuildProgram(program, 1, &this->resources->deviceId, nullptr, nullptr, nullptr);
    std::cout << "status clBuildProgram: " << status << std::endl;
    this->resources->programs.push_back(program);
    return program;
  }
}