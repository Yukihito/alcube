#include "ProgramFactory.h"

namespace alcube::utils::opencl {
  ProgramFactory::ProgramFactory(Resources *resources, FileUtil *fileUtil) {
    this->resources = resources;
    this->fileUtil = fileUtil;
  }

  cl_program ProgramFactory::create(const char* path) {
    return createFromRawStringCode(fileUtil->readFile(path).c_str());
  }

  cl_program ProgramFactory::create(std::initializer_list<const char *> paths) {
    std::string str;
    for (auto path: paths) {
      str.append(fileUtil->readFile(path));
      str.append("\n");
    }
    return createFromRawStringCode(str.c_str());
  }

  cl_program ProgramFactory::createFromRawStringCode(const char *str) {
    cl_int status;
    const char *src[] = {str};
    cl_program program = clCreateProgramWithSource(this->resources->context, 1, (const char **) &src, nullptr, nullptr);
    status = clBuildProgram(program, 1, &this->resources->deviceId, "-Werror", nullptr, nullptr);
    if (status != CL_SUCCESS) {
      std::cerr << "status clBuildProgram: " << status << std::endl;
      size_t length;
      char buffer[65536];
      clGetProgramBuildInfo(program, this->resources->deviceId, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &length);
      std::cerr << "--- Build log ---\n " << buffer << std::endl;
      exit(1);
    }
    this->resources->programs.push_back(program);
    return program;
  }
}