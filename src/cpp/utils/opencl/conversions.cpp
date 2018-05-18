#include "conversions.h"

namespace alcube::utils::opencl::conversions {

  cl_float3 toCl(glm::vec3 v) {
    return {v.x, v.y, v.z};
  }

  cl_float4 toCl(glm::quat v) {
    return {v.x, v.y, v.z, v.w};
  }

  glm::vec3 toGlm(cl_float3 v) {
    return glm::vec3(v.s[0], v.s[1], v.s[2]);
  }

  glm::quat toGlmQuat(cl_float4 v) {
    return glm::quat(v.s[3], v.s[0], v.s[1], v.s[2]);
  }
}