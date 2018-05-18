#ifndef ALCUBE_UTIL_OPENCL_CONVERSIONS_H
#define ALCUBE_UTIL_OPENCL_CONVERSIONS_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <CL/cl.h>

namespace alcube::utils::opencl::conversions {
  cl_float3 toCl(glm::vec3 v);
  cl_float4 toCl(glm::quat v);
  glm::vec3 toGlm(cl_float3 v);
  glm::quat toGlmQuat(cl_float4 v);
}

#endif //ALCUBE_UTIL_OPENCL_CONVERSIONS_H
