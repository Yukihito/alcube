#ifndef ALCUBE_UTIL_OPENCL_CONVERSIONS_H
#define ALCUBE_UTIL_OPENCL_CONVERSIONS_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <OpenCL/opencl.h>

namespace alcube::utils::opencl::conversions {
  void assignClFloat3(cl_float3& clv, glm::vec3& glmv);
  void assignClFloat4(cl_float4& clv, glm::quat& glmv);
  void assignGlmVec3(glm::vec3& glmv, cl_float3& clv);
  void assignGlmQuat(glm::quat& glmv, cl_float4& clv);
}

#endif //ALCUBE_UTIL_OPENCL_CONVERSIONS_H
