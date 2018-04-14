#include "Resources.h"

namespace alcube::utils::opencl {
  Resources::Resources() {
    cl_int status;

    size_t size;
    status = clGetPlatformIDs(1, &platformId, &platformsCount);
    // std::cout << "status clGetPlatformIds: " << status << std::endl;
    // std::cout << "platformId: " << platformId << std::endl;
    // std::cout << "platformsCount: " << platformsCount << std::endl;
    status = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, 0, nullptr, &size);
    char value[size];
    status = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, size, &value, nullptr);
    // std::cout << value << std::endl;
    status = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, &devicesCount);
    // std::cout << "status clGetDeviceIds: " << status << std::endl;
    // std::cout << "deviceId: " << deviceId << std::endl;
    // std::cout << "devicesCount: " << devicesCount << std::endl;
    cl_uint addressBits;
    clGetDeviceInfo(deviceId, CL_DEVICE_ADDRESS_BITS, 0, nullptr, &size);
    clGetDeviceInfo(deviceId, CL_DEVICE_ADDRESS_BITS, size, &addressBits, nullptr);
    // std::cout << "address bits: " << addressBits << std::endl;
    context = clCreateContext(nullptr, 1, &deviceId, nullptr, nullptr, nullptr);

    memoryManager = new utils::opencl::MemoryManager();
    memoryManager->context = context;
  }

  void Resources::release() {
    std::cout << "release 0" << std::endl;
    memoryManager->release();
    std::cout << "release 1" << std::endl;
    for (cl_kernel kernel : kernels) {
      clReleaseKernel(kernel);
    }
    std::cout << "release 2" << std::endl;
    for (cl_program program: programs) {
      clReleaseProgram(program);
    }
    std::cout << "release 3" << std::endl;
    for (cl_command_queue queue: queues) {
      std::cout << "before release 3" << std::endl;
      clReleaseCommandQueue(queue);
      std::cout << "after release 3" << std::endl;
    }
    std::cout << "release 4" << std::endl;
    clReleaseContext(context);
  }
}