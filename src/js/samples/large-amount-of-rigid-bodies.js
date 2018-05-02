var ball, ballCount, i, j, physicsFeatures, ref, renderingGroups, w;

renderingGroups = {
  texture: renderingGroupFactory.create()
};

renderingGroups.texture.diffuse(vec3(1.0, 1.0, 1.0));

renderingGroups.texture.ambient(vec3(0.3, 0.3, 0.3));

renderingGroups.texture.specular(vec3(0.1, 0.1, 0.1));

renderingGroups.texture.instanceColorType(InstanceColorType.RANDOM);

renderingGroups.texture.texture(Texture.CHECK);

cube.add(renderingGroups.texture);

physicsFeatures = softbodyFeaturesFactory.create();

physicsFeatures.elasticity(0.8);

ball = function(position) {
  var actor;
  actor = actorFactory.create(physicsFeatures);
  actor.position(position);
  cube.add(actor);
  renderingGroups.texture.add(actor);
  return actor;
};

ballCount = 128;

w = 32;

for (i = j = 0, ref = ballCount; undefined !== 0 && (0 <= ref ? 0 <= j && j < ref : 0 >= j && j > ref); i = 0 <= ref ? ++j : --j) {
  ball(vec3Rand().multiplyScalar(w).add(vec3(1, 1, 1).multiplyScalar(-w / 2)));
}
