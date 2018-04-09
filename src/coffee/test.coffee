fluidFeatures = fluidFeaturesFactory.create()

fluid = (position) ->
  actor = actorFactory.create fluidFeatures
  actor.position position
  actor

softbodyFeatures = softbodyFeaturesFactory.create()
softbodyFeatures.elasticity 0.0
softbodyFeatures.mass 0.2

softbodies = []
softbody = (position, linearMomentum) ->
  actor = actorFactory.create softbodyFeatures
  actor.position position
  actor.linearMomentum linearMomentum
  print vec3ToArray actor.linearMomentum()
  softbodies.push actor
  actor

springFactory.k 64
spring = (actor0, position0, actor1, position1) ->
  springFactory.create actor0, position0, actor1, position1

w = 64

for i in [0...32]
  for j in [0...i]
    for k in [0...i]
      pos = vec3(i, j, k).multiplyScalar(2)
        .add vec3(1, 1, 1).multiplyScalar(1 - w/2)
        .add vec3Rand().multiplyScalar(0.01)
      cube.add fluid pos


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
