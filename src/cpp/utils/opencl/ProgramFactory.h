#ifndef ALCUBE_OPENCL_PROGRAMFACTORY_H
#define ALCUBE_OPENCL_PROGRAMFACTORY_H

#include <OpenCL/opencl.h>
#include <string>
#include "Resources.h"
#include "../FileUtil.h"

namespace alcube::utils::opencl {
  class ProgramFactory {
    public:
      ProgramFactory(Resources* resources, FileUtil* fileUtil);
      cl_program create(const char* path);

    private:
      FileUtil* fileUtil;
      Resources* resources;
  };
}
#endif //ALCUBE_OPENCL_PROGRAMFACTORY_H
