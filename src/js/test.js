var a, fluid, fluidFeatures;

fluidFeatures = fluidFeaturesFactory.create();

fluid = function(position) {
  var actor;
  actor = actorFactory.create(fluidFeatures);
  actor.position(position);
  return actor;
};

a = fluid(vec3(1.1, 2.2, 3.3));

print(a.position().x);
