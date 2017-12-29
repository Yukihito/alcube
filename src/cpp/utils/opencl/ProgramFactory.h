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
      cl_program create(std::initializer_list<const char*> paths);

    private:
      cl_program createFromRawStringCode(const char* str);
      FileUtil* fileUtil;
      Resources* resources;
  };
}
#endif //ALCUBE_OPENCL_PROGRAMFACTORY_H
