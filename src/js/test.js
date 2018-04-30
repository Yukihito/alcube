var fluid, fluidFeatures, i, j, k, l, linearMomentum, m, n, o, p, pos, q, r, ref, ref1, ref2, ref3, ref4, ref5, ref6, ref7, renderer, renderer2, s, softbodies, softbody, softbodyEdgeLength, softbodyFeatures, softbodySize, spring, t, u, w, x, y, z;

renderer = rendererFactory.create();

renderer.diffuse(vec3(1.0, 1.0, 1.0));

renderer.ambient(vec3(0.3, 0.3, 0.3));

renderer.specular(vec3(0.1, 0.1, 0.1));

renderer.instanceColorType(InstanceColorType.LINEAR_MOMENTUM);

renderer2 = rendererFactory.create();

renderer2.diffuse(vec3(1.0, 1.0, 1.0));

renderer2.ambient(vec3(0.3, 0.3, 0.3));

renderer2.specular(vec3(0.1, 0.1, 0.1));

renderer2.instanceColorType(InstanceColorType.NONE);

fluidFeatures = fluidFeaturesFactory.create();

fluid = function(position) {
  var actor;
  actor = actorFactory.create(fluidFeatures, renderer2);
  actor.position(position);
  return actor;
};

softbodyFeatures = softbodyFeaturesFactory.create();

softbodyFeatures.elasticity(0.0);

softbodyFeatures.mass(0.2);

softbodies = [];

softbody = function(position, linearMomentum) {
  var actor;
  actor = actorFactory.create(softbodyFeatures, renderer);
  actor.position(position);
  actor.linearMomentum(linearMomentum);
  softbodies.push(actor);
  return actor;
};

springFactory.k(64);

spring = function(actor0, position0, actor1, position1) {
  return springFactory.create(actor0, position0, actor1, position1);
};

w = 64;

for (i = l = 0; l < 32; i = ++l) {
  for (j = m = 0, ref = i; undefined !== 0 && (0 <= ref ? 0 <= m && m < ref : 0 >= m && m > ref); j = 0 <= ref ? ++m : --m) {
    for (k = n = 0, ref1 = i; undefined !== 0 && (0 <= ref1 ? 0 <= n && n < ref1 : 0 >= n && n > ref1); k = 0 <= ref1 ? ++n : --n) {
      pos = vec3(i, j, k).multiplyScalar(2).add(vec3(1, 1, 1).multiplyScalar(1 - w / 2)).add(vec3Rand().multiplyScalar(0.01));
      cube.add(fluid(pos));
    }
  }
}

softbodySize = 12;

softbodyEdgeLength = softbodySize * 2;

for (z = o = 0, ref2 = softbodySize; undefined !== 0 && (0 <= ref2 ? 0 <= o && o < ref2 : 0 >= o && o > ref2); z = 0 <= ref2 ? ++o : --o) {
  for (y = p = 0, ref3 = softbodySize; undefined !== 0 && (0 <= ref3 ? 0 <= p && p < ref3 : 0 >= p && p > ref3); y = 0 <= ref3 ? ++p : --p) {
    for (x = q = 0, ref4 = softbodySize; undefined !== 0 && (0 <= ref4 ? 0 <= q && q < ref4 : 0 >= q && q > ref4); x = 0 <= ref4 ? ++q : --q) {
      pos = vec3(x, y, z).multiplyScalar(2).add(vec3(1, 1, 1).multiplyScalar(-softbodyEdgeLength / 2)).add(vec3(-w / 4, w / 4, 0));
      linearMomentum = vec3(3, 1, 0.5);
      cube.add(softbody(pos, linearMomentum));
    }
  }
}

i = 0;

for (z = r = 0, ref5 = softbodySize; undefined !== 0 && (0 <= ref5 ? 0 <= r && r < ref5 : 0 >= r && r > ref5); z = 0 <= ref5 ? ++r : --r) {
  for (y = t = 0, ref6 = softbodySize; undefined !== 0 && (0 <= ref6 ? 0 <= t && t < ref6 : 0 >= t && t > ref6); y = 0 <= ref6 ? ++t : --t) {
    for (x = u = 0, ref7 = softbodySize; undefined !== 0 && (0 <= ref7 ? 0 <= u && u < ref7 : 0 >= u && u > ref7); x = 0 <= ref7 ? ++u : --u) {
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

renderer.setUpResources();

renderer2.setUpResources();
