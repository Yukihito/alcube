renderingGroups =
  texture : renderingGroupFactory.create()

renderingGroups.texture.diffuse vec3 1.0, 1.0, 1.0
renderingGroups.texture.ambient vec3 0.3, 0.3, 0.3
renderingGroups.texture.specular vec3 0.1, 0.1, 0.1
renderingGroups.texture.instanceColorType InstanceColorType.RANDOM
renderingGroups.texture.texture Texture.CHECK
cube.add renderingGroups.texture

physicsFeatures = softbodyFeaturesFactory.create()
physicsFeatures.elasticity 0.8

ball = (position) ->
  actor = actorFactory.create physicsFeatures
  actor.position position
  cube.add actor
  renderingGroups.texture.add actor
  actor

ballCount = 128

w = 32

for i in [0...ballCount]
  ball vec3Rand().multiplyScalar(w).add vec3(1, 1, 1).multiplyScalar(-w/2)
