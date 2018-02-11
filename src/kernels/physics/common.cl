#define PARTICLE_TYPE_RIGID_BODY      = 0
#define PARTICLE_TYPE_FACE            = 1
#define PARTICLE_TYPE_SPHERICAL_SHELL = 2
#define PARTICLE_TYPE_FLUID           = 3

float4 mulQuat(float4 q, float4 r);
float3 rotateByQuat(float3 v, float4 q);
float4 createQuatFromDisplacement(float3 angularDisplacement);

float4 mulQuat(float4 q, float4 r) {
  float4 result;
  result.xyz = r.xyz * q.w + q.xyz * r.w + cross(q.xyz, r.xyz);
  result.w = q.w * r.w - dot(q.xyz, r.xyz);
  return result;
}

float3 rotateByQuat(float3 v, float4 q) {
  return mulQuat(mulQuat(q, (float4)(v.x, v.y, v.z, 0.0f)), (float4)(-q.x, -q.y, -q.z, q.w)).xyz;
}

float4 createQuatFromDisplacement(float3 angularDisplacement) {
  float halfRotationScalar = length(angularDisplacement) / 2.0f;
  float4 q;
  q.w = cos(halfRotationScalar);
  q.xyz = normalize(angularDisplacement) * sin(halfRotationScalar);
  return q;
}
