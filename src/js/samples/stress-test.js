var ball, ballCount, i, j, physicsFeatures, ref, renderingGroups, w;

renderingGroups = {
  simple: renderingGroupFactory.create()
};

renderingGroups.simple.diffuse(vec3(1.0, 1.0, 1.0));

renderingGroups.simple.ambient(vec3(0.3, 0.3, 0.3));

renderingGroups.simple.specular(vec3(0.1, 0.1, 0.1));

renderingGroups.simple.instanceColorType(InstanceColorType.RANDOM);

renderingGroups.simple.texture(Texture.NONE);

cube.add(renderingGroups.simple);

physicsFeatures = softbodyFeaturesFactory.create();

physicsFeatures.elasticity(0.8);

ball = function(position, linearMomentum) {
  var actor;
  actor = actorFactory.create(physicsFeatures);
  actor.position(position);
  actor.linearMomentum(linearMomentum);
  cube.add(actor);
  renderingGroups.simple.add(actor);
  return actor;
};

ballCount = 65536;

w = 512;

for (i = j = 0, ref = ballCount; undefined !== 0 && (0 <= ref ? 0 <= j && j < ref : 0 >= j && j > ref); i = 0 <= ref ? ++j : --j) {
  ball(vec3Rand().multiplyScalar(w).add(vec3(1, 1, 1).multiplyScalar(-w / 2)), vec3Rand().add(vec3(-0.5, -0.5, -0.5)).multiplyScalar(30));
}
