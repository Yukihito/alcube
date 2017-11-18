typedef struct __attribute__ ((packed)) GridStruct {
  unsigned int xCount;
  unsigned int yCount;
  unsigned int zCount;
  unsigned int edgeLength;
  float3 origin;
} Grid;

typedef struct __attribute__ ((packed)) GridAndCellRelationStruct {
  unsigned int gridIndex;
  unsigned short cellIndex;
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
  float weight;
  unsigned int springStartIndex;
  unsigned int springEndIndex;
} Cell;

__kernel void initGridAndCellRelations(
  __global GridAndCellRelation* relations,
  const uint gridIndex,
  const unsigned short cellIndex
) {
  size_t i = get_global_id(0);
  relations[i].gridIndex = gridIndex;
  relations[i].cellIndex = cellIndex;
}

__kernel void fillGridIndex(
  __global const Grid* grid,
  __global const Cell* cells,
  __global RigidBodyState* currentStates,
  __global GridAndCellRelation* relations
) {
  size_t cellIndex = get_global_id(0);
  float edgeLength = grid->edgeLength;
  float3 position = currentStates[cellIndex].position;
  float3 positionGridSpace = position - grid->origin;
  unsigned int gridIndex = (int)(positionGridSpace.x / edgeLength) + (int)(positionGridSpace.y / edgeLength) * grid->xCount + (int)(positionGridSpace.z / edgeLength) * grid->xCount * grid->yCount;
  currentStates[cellIndex].gridIndex = gridIndex;
  relations[cellIndex].cellIndex = cellIndex;
  relations[cellIndex].gridIndex = gridIndex;
}

__kernel void merge(
  __global GridAndCellRelation* relations,
  const uint distance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    GridAndCellRelation left = relations[index];
    GridAndCellRelation right = relations[index + distance];

    // Original code
    /*
    ushort cmpMask = left.gridIndex < right.gridIndex;
    uint cmpMaskInt = (uint)cmpMask;

    relations[index].gridIndex = select(left.gridIndex, right.gridIndex, cmpMaskInt);
    relations[index].cellIndex = select(left.cellIndex, right.cellIndex, cmpMask);
    relations[index + distance].gridIndex = select(right.gridIndex, left.gridIndex, cmpMaskInt);
    relations[index + distance].cellIndex = select(right.cellIndex, left.cellIndex, cmpMask);
    */
    bool b = left.gridIndex < right.gridIndex;
    relations[index] = b ? left : right;
    relations[index + distance] = b ? right : left;
  }
}

__kernel void bitonic(
  __global GridAndCellRelation* relations,
  const uint distance,
  const uint stageDistance
) {
  uint index = get_global_id(0);
  if (index % (distance << 1) < distance) {
    uint middleDistance = stageDistance << 1; // * 2
    GridAndCellRelation left = relations[index];
    GridAndCellRelation right = relations[index + distance];
    /*
    // Original code
    uint cmpMask = left.gridIndex < right.gridIndex ? 1 : 0;
    if (index % (middleDistance << 1) >= middleDistance) {
      cmpMask = left.gridIndex < right.gridIndex ? 1 : 0;
    } else {
      cmpMask = left.gridIndex > right.gridIndex ? 1 : 0;
    }
    relations[index].gridIndex = select(left.gridIndex, right.gridIndex, cmpMaskInt);
    relations[index].cellIndex = select(left.cellIndex, right.cellIndex, cmpMask);
    relations[index + distance].gridIndex = select(right.gridIndex, left.gridIndex, cmpMaskInt);
    relations[index + distance].cellIndex = select(right.cellIndex, left.cellIndex, cmpMask);
    */

    /*
    // Improvement 1
    ushort cmpMask = !(b1 ^ b2);
    uint cmpMaskInt = (uint)cmpMask;
    relations[index].gridIndex = select(left.gridIndex, right.gridIndex, cmpMaskInt);
    relations[index].cellIndex = select(left.cellIndex, right.cellIndex, cmpMask);
    relations[index + distance].gridIndex = select(right.gridIndex, left.gridIndex, cmpMaskInt);
    relations[index + distance].cellIndex = select(right.cellIndex, left.cellIndex, cmpMask);
    */
    bool b1 = left.gridIndex < right.gridIndex;
    bool b2 = index % (middleDistance << 1) >= middleDistance;
    bool b3 = !(b1 ^ b2);

    relations[index] = b3 ? left : right;
    relations[index + distance] = b3 ? right : left;
  }
}

__kernel void setGridRelationIndexRange(
  __global GridAndCellRelation* relations,
  __global uint* gridStartIndices,
  __global uint* gridEndIndices
) {
  uint current = get_global_id(0);
  uint next = current + 1;
  uint currentGridIndex = relations[current].gridIndex;
  uint nextGridIndex = relations[next].gridIndex;
  if (currentGridIndex != nextGridIndex) {
    gridEndIndices[currentGridIndex] = current;
    gridStartIndices[nextGridIndex] = next;
  }
}
