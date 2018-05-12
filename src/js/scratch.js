var i, j, pos;

for (i = j = 0; j < 128; i = ++j) {
  pos = vec3Rand().multiplyScalar(10);
  fluid(pos);
}
