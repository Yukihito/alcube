var fluid, fluidFeatures, i, j, k, l, m, n, pos, ref, ref1, renderingGroups, w;

renderingGroups = {
  fluid: renderingGroupFactory.create()
};

renderingGroups.fluid.diffuse(vec3(0.5, 0.5, 1.0));

renderingGroups.fluid.ambient(vec3(0.1, 0.1, 0.3));

renderingGroups.fluid.specular(vec3(0.1, 0.1, 0.1));

renderingGroups.fluid.instanceColorType(InstanceColorType.NONE);

cube.add(renderingGroups.fluid);

fluidFeatures = fluidFeaturesFactory.create();

fluid = function(position) {
  var actor;
  actor = actorFactory.create(fluidFeatures);
  actor.position(position);
  renderingGroups.fluid.add(actor);
  return actor;
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
