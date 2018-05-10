var fluid, fluidFeatures, linearMomentum, pos, renderingGroups, softbodies, softbody, softbodyFeatures, spring;

renderingGroups = {
  fluid: renderer.createGroup(),
  softbody: renderer.createGroup()
};

renderingGroups.fluid.diffuse(vec3(0.5, 0.5, 1.0));

renderingGroups.fluid.ambient(vec3(0.1, 0.1, 0.3));

renderingGroups.fluid.specular(vec3(0.1, 0.1, 0.1));

renderingGroups.fluid.instanceColorType(InstanceColorType.NONE);

renderingGroups.softbody.diffuse(vec3(1.0, 1.0, 1.0));

renderingGroups.softbody.ambient(vec3(0.3, 0.3, 0.3));

renderingGroups.softbody.specular(vec3(0.1, 0.1, 0.1));

renderingGroups.softbody.instanceColorType(InstanceColorType.LINEAR_MOMENTUM);

fluidFeatures = fluidFeaturesFactory.create();

fluid = function(position) {
  var actor;
  actor = actorFactory.create(fluidFeatures, renderingGroups.fluid);
  actor.position(position);
  return actor;
};

softbodyFeatures = softbodyFeaturesFactory.create();

softbodyFeatures.elasticity(0.0);

softbodyFeatures.mass(0.2);

softbodies = [];

softbody = function(position, linearMomentum) {
  var actor;
  actor = actorFactory.create(softbodyFeatures, renderingGroups.softbody);
  actor.position(position);
  actor.linearMomentum(linearMomentum);
  softbodies.push(actor);
  return actor;
};

springFactory.k(64);

spring = function(actor0, position0, actor1, position1) {
  return springFactory.create(actor0, position0, actor1, position1);
};

linearMomentum = vec3(3, 1, 0.5);

pos = vec3(0, 0, 0);

softbody(pos, linearMomentum);

fluid(pos);

print(testbuf);
