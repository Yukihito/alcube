#ifndef ALCUBE_OPENCL_KERNELFACTORY_H
#define ALCUBE_OPENCL_KERNELFACTORY_H

#include <OpenCL/opencl.h>
#include <string>
#include "Resources.h"
#include "../FileUtil.h"

namespace alcube::utils::opencl {

  class KernelFactory {
    public:
      KernelFactory(Resources* resources, FileUtil* fileUtil);
      cl_kernel create(cl_program program, const char *name);

    private:
      Resources* resources;
  };
}

#endif //ALCUBE_OPENCL_KERNELFACTORY_H
