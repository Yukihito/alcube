renderingGroups =
  simple : renderingGroupFactory.create()

renderingGroups.simple.diffuse vec3 1.0, 1.0, 1.0
renderingGroups.simple.ambient vec3 0.3, 0.3, 0.3
renderingGroups.simple.specular vec3 0.1, 0.1, 0.1
renderingGroups.simple.instanceColorType InstanceColorType.RANDOM
renderingGroups.simple.texture Texture.NONE
cube.add renderingGroups.simple

physicsFeatures = softbodyFeaturesFactory.create()
physicsFeatures.elasticity 0.8

ball = (position, linearMomentum) ->
  actor = actorFactory.create physicsFeatures
  actor.position position
  actor.linearMomentum linearMomentum
  cube.add actor
  renderingGroups.simple.add actor
  actor

ballCount = 65536

w = 512

for i in [0...ballCount]
  ball vec3Rand().multiplyScalar(w).add(vec3(1, 1, 1).multiplyScalar(-w/2)), vec3Rand().add(vec3(-0.5, -0.5, -0.5)).multiplyScalar(30)
