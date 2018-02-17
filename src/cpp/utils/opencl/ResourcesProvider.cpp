#include "ResourcesProvider.h"
namespace alcube::utils::opencl {
  ResourcesProvider::ResourcesProvider(FileUtil *fileUtil, Resources* resources) {
    kernelFactory = new utils::opencl::KernelFactory(resources, fileUtil);
    programFactory = new utils::opencl::ProgramFactory(resources, fileUtil);
    memoryManager = new utils::opencl::MemoryManager(resources);
    queue = new utils::opencl::CommandQueue(resources);
    this->resources = resources;
  }
}