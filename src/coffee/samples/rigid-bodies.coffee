renderingGroups =
  texture : renderingGroupFactory.create()

renderingGroups.texture.diffuse vec3 1.0, 1.0, 1.0
renderingGroups.texture.ambient vec3 0.3, 0.3, 0.3
renderingGroups.texture.specular vec3 0.1, 0.1, 0.1
renderingGroups.texture.instanceColorType InstanceColorType.RANDOM
renderingGroups.texture.texture Texture.CHECK
cube.add renderingGroups.texture

physicsFeatures = softbodyFeaturesFactory.create()

ball = (position, linearMomentum) ->
  actor = actorFactory.create physicsFeatures
  actor.position position
  actor.linearMomentum linearMomentum
  cube.add actor
  renderingGroups.texture.add actor
  actor

ball vec3(0, 0, 0), vec3(0, 0, 0)
ball vec3(3, 0, 0), vec3(-5, 0.1, 0)