typedef struct __attribute__ ((packed)) GridStruct {
  unsigned int xCount;
  unsigned int yCount;
  unsigned int zCount;
  unsigned int edgeLength;
  float3 origin;
  float3 normals[6];
} Grid;

typedef struct __attribute__ ((packed)) GridAndCellRelationStruct {
  unsigned int gridIndex;
  unsigned short cellIndex;
  short padding;
} GridAndCellRelation;

typedef struct __attribute__ ((packed)) RigidBodyStateStruct {
  float3 position;
  float4 rotation;
  float3 linearMomentum;
  float3 angularMomentum;
  unsigned int gridIndex;
} RigidBodyState;

typedef struct __attribute__ ((packed)) CellStruct {
  float radius;
  float mass;
  float elasticity;
  float dynamicFrictionCoefficient;
  float staticFrictionCoefficient;
} Cell;

typedef struct __attribute__ ((packed)) IntersectionStruct {
  unsigned short type;
  unsigned short otherIndex;
  float length;
  float speed;
  float3 normal;
} Intersection;

typedef struct __attribute__ ((packed)) CellVarStruct {
  float3 linearVelocity;
  float3 angularVelocity;
  float momentOfInertia;
  float massForIntersection;
  float massForCollision;
  int isFloating;
  unsigned short intersectionCount;
  unsigned short collisionCount;
  unsigned char collisionIndices[16];
  Intersection intersections[16];
} CellVar;

float4 mulQuat(float4* q, float4* r);
float3 rotateByQuat(float3* v, float4* q);

void accumulatePenaltyImpulse(
  __global Intersection* intersection,
  const float deltaTime,
  float3* acc
);

void accumulateFrictionalImpulse(
  __global const Cell* cell,
  __global CellVar* cellVar,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global Intersection* intersection,
  float3* linearMomentumAcc,
  float3* angularMomentumAcc
);

void accumulateConstraintImpulse(
  __global const Cell* cell,
  __global CellVar* cellVar,
  __global const Cell* cells,
  __global CellVar* cellVars,
  __global Intersection* intersection,
  float3* acc
);

float4 mulQuat(
  float4* q,
  float4* r
) {
  float3 u = (float3)(q->x, q->y, q->z);
  float3 v = (float3)(r->x, r->y, r->z);
  float3 ipart = v * q->w + u * r->w + cross(u, v);
  float tpart = q->w * r->w - dot(u, v);
  return (float4)(ipart.x, ipart.y, ipart.z, tpart);
}

float3 rotateByQuat(
  float3* v,
  float4* q
) {
  float4 p = (float4)(v->x, v->y, v->z, 0.0f);
  float4 r = (float4)(-q->x, -q->y, -q->z, q->w);
  float4 qp = mulQuat(q, &p);
  float4 qpr = mulQuat(&qp, &r);
  return (float3)(qpr.x, qpr.y, qpr.z);
}
