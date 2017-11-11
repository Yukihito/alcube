#include "conversions.h"

namespace alcube::utils::opencl::conversions {
  void assignClFloat3(cl_float3 &clv, glm::vec3 &glmv) {
    clv.s[0] = glmv.x;
    clv.s[1] = glmv.y;
    clv.s[2] = glmv.z;
  }

  void assignClFloat4(cl_float4 &clv, glm::quat &glmv) {
    clv.s[0] = glmv.x;
    clv.s[1] = glmv.y;
    clv.s[2] = glmv.z;
    clv.s[3] = glmv.w;
  }

  void assignGlmVec3(glm::vec3 &glmv, cl_float3 &clv) {
    glmv.x = clv.s[0];
    glmv.y = clv.s[1];
    glmv.z = clv.s[2];
  }

  void assignGlmQuat(glm::quat &glmv, cl_float3 &clv) {
    glmv.x = clv.s[0];
    glmv.y = clv.s[1];
    glmv.z = clv.s[2];
    glmv.w = clv.s[3];
  }
}