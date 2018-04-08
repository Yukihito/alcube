var a, fluid, fluidFeatures;

fluidFeatures = new FluidFeatures();

fluid = function(position) {
  var actor;
  actor = actorFactory.create(fluidFeatures);
  actor.position(position);
  return actor;
};

a = fluid(vec3(1.1, 2.2, 3.3));

a.position().x;
