typedef struct __attribute__ ((packed)) JunctionNode {
  float horizontalPosition;
  float verticalPosition;
}

typedef struct __attribute__ ((packed)) BioticState {
  float radius;
  float mass;
  float elasticity;
  float dynamicFrictionCoefficient;
  float staticFrictionCoefficient;
  uint springIndices[16];
  uchar springNodeIndices[16];
  uint springCount;
  int alterEgoIndex;
  float radiusForAlterEgo;
} Cell;
