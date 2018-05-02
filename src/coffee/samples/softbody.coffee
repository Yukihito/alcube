renderingGroups =
  softbody: renderingGroupFactory.create()

renderingGroups.softbody.diffuse vec3 1.0, 1.0, 1.0
renderingGroups.softbody.ambient vec3 0.3, 0.3, 0.3
renderingGroups.softbody.specular vec3 0.1, 0.1, 0.1
renderingGroups.softbody.instanceColorType InstanceColorType.LINEAR_MOMENTUM

cube.add renderingGroups.softbody

softbodyFeatures = softbodyFeaturesFactory.create()
softbodyFeatures.elasticity 0.0
softbodyFeatures.mass 0.2

softbodies = []
softbody = (position, linearMomentum) ->
  actor = actorFactory.create softbodyFeatures
  actor.position position
  actor.linearMomentum linearMomentum
  softbodies.push actor
  renderingGroups.softbody.add actor
  actor

springFactory.k 64
spring = (actor0, position0, actor1, position1) ->
  springFactory.create actor0, position0, actor1, position1

w = 64

softbodySize = 12
softbodyEdgeLength = softbodySize * 2
for z in [0...softbodySize]
  for y in [0...softbodySize]
    for x in [0...softbodySize]
      pos = vec3(x, y, z).multiplyScalar(2)
        .add vec3(1, 1, 1).multiplyScalar(-softbodyEdgeLength/2)
        .add vec3(-w/4, w/4, 0)
      linearMomentum = vec3 3, 1, 0.5
      cube.add softbody(pos, linearMomentum)

i = 0
for z in [0...softbodySize]
  for y in [0...softbodySize]
    for x in [0...softbodySize]
      if x + 1 < softbodySize
        s = spring softbodies[i], vec3(1, 0, 0), softbodies[i + 1], vec3(-1, 0, 0)
        cube.add s
      if y + 1 < softbodySize
        s = spring softbodies[i], vec3(0, 1, 0), softbodies[i + softbodySize], vec3(0, -1, 0)
        cube.add s
      if z + 1 < softbodySize
        s = spring softbodies[i], vec3(0, 0, 1), softbodies[i + softbodySize * softbodySize], vec3(0, 0, -1)
        cube.add s
      i++
