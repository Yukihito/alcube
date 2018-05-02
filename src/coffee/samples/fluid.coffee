renderingGroups =
  fluid: renderingGroupFactory.create()

renderingGroups.fluid.diffuse vec3 0.5, 0.5, 1.0
renderingGroups.fluid.ambient vec3 0.1, 0.1, 0.3
renderingGroups.fluid.specular vec3 0.1, 0.1, 0.1
renderingGroups.fluid.instanceColorType InstanceColorType.NONE

cube.add renderingGroups.fluid

fluidFeatures = fluidFeaturesFactory.create()

fluid = (position) ->
  actor = actorFactory.create fluidFeatures
  actor.position position
  renderingGroups.fluid.add actor
  actor

w = 64

for i in [0...32]
  for j in [0...i]
    for k in [0...i]
      pos = vec3(i, j, k).multiplyScalar(2)
        .add vec3(1, 1, 1).multiplyScalar(1 - w/2)
        .add vec3Rand().multiplyScalar(0.01)
      cube.add fluid pos
