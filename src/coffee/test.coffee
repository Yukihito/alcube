fluidFeatures = fluidFeaturesFactory.create()

fluid = (position) ->
  actor = actorFactory.create fluidFeatures
  actor.position position
  actor

a = fluid vec3 1.1, 2.2, 3.3
print a.position().x
