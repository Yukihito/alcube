#ifndef ALCUBE_OPENCL_RESOURCES_H
#define ALCUBE_OPENCL_RESOURCES_H

#include <CL/cl.h>
#include <iostream>
#include <vector>
#include "MemoryManager.h"

namespace alcube::utils::opencl {
  class Resources {
    public:
      cl_uint platformsCount;
      cl_platform_id platformId;
      cl_uint devicesCount;
      cl_device_id deviceId;
      cl_context context;
      std::vector<cl_kernel> kernels = {};
      std::vector<cl_command_queue> queues = {};
      std::vector<cl_program> programs = {};
      MemoryManager* memoryManager;

      explicit Resources();
      void release();
  };
}

#endif //ALCUBE_OPENCL_RESOURCES_H
