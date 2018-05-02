var i, j, k, l, linearMomentum, m, n, o, pos, ref, ref1, ref2, ref3, ref4, ref5, renderingGroups, s, softbodies, softbody, softbodyEdgeLength, softbodyFeatures, softbodySize, spring, w, x, y, z;

renderingGroups = {
  softbody: renderingGroupFactory.create()
};

renderingGroups.softbody.diffuse(vec3(1.0, 1.0, 1.0));

renderingGroups.softbody.ambient(vec3(0.3, 0.3, 0.3));

renderingGroups.softbody.specular(vec3(0.1, 0.1, 0.1));

renderingGroups.softbody.instanceColorType(InstanceColorType.LINEAR_MOMENTUM);

cube.add(renderingGroups.softbody);

softbodyFeatures = softbodyFeaturesFactory.create();

softbodyFeatures.elasticity(0.0);

softbodyFeatures.mass(0.2);

softbodies = [];

softbody = function(position, linearMomentum) {
  var actor;
  actor = actorFactory.create(softbodyFeatures);
  actor.position(position);
  actor.linearMomentum(linearMomentum);
  softbodies.push(actor);
  renderingGroups.softbody.add(actor);
  return actor;
};

springFactory.k(64);

spring = function(actor0, position0, actor1, position1) {
  return springFactory.create(actor0, position0, actor1, position1);
};

w = 64;

softbodySize = 12;

softbodyEdgeLength = softbodySize * 2;

for (z = j = 0, ref = softbodySize; undefined !== 0 && (0 <= ref ? 0 <= j && j < ref : 0 >= j && j > ref); z = 0 <= ref ? ++j : --j) {
  for (y = k = 0, ref1 = softbodySize; undefined !== 0 && (0 <= ref1 ? 0 <= k && k < ref1 : 0 >= k && k > ref1); y = 0 <= ref1 ? ++k : --k) {
    for (x = l = 0, ref2 = softbodySize; undefined !== 0 && (0 <= ref2 ? 0 <= l && l < ref2 : 0 >= l && l > ref2); x = 0 <= ref2 ? ++l : --l) {
      pos = vec3(x, y, z).multiplyScalar(2).add(vec3(1, 1, 1).multiplyScalar(-softbodyEdgeLength / 2)).add(vec3(-w / 4, w / 4, 0));
      linearMomentum = vec3(3, 1, 0.5);
      cube.add(softbody(pos, linearMomentum));
    }
  }
}

i = 0;

for (z = m = 0, ref3 = softbodySize; undefined !== 0 && (0 <= ref3 ? 0 <= m && m < ref3 : 0 >= m && m > ref3); z = 0 <= ref3 ? ++m : --m) {
  for (y = n = 0, ref4 = softbodySize; undefined !== 0 && (0 <= ref4 ? 0 <= n && n < ref4 : 0 >= n && n > ref4); y = 0 <= ref4 ? ++n : --n) {
    for (x = o = 0, ref5 = softbodySize; undefined !== 0 && (0 <= ref5 ? 0 <= o && o < ref5 : 0 >= o && o > ref5); x = 0 <= ref5 ? ++o : --o) {
      if (x + 1 < softbodySize) {
        s = spring(softbodies[i], vec3(1, 0, 0), softbodies[i + 1], vec3(-1, 0, 0));
        cube.add(s);
      }
      if (y + 1 < softbodySize) {
        s = spring(softbodies[i], vec3(0, 1, 0), softbodies[i + softbodySize], vec3(0, -1, 0));
        cube.add(s);
      }
      if (z + 1 < softbodySize) {
        s = spring(softbodies[i], vec3(0, 0, 1), softbodies[i + softbodySize * softbodySize], vec3(0, 0, -1));
        cube.add(s);
      }
      i++;
    }
  }
}
