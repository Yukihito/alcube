vec3 = (x, y, z) ->
  new THREE.Vector3(x, y, z);

class FluidFeatures
  constructor: () ->
    @underlying = createActorFactory()
    @stiffness = 64.0
    @density = 0.02
    @viscosity = 8.0

  createActor: () =>
    @underlying.createFluid(this)


class Actor
  constructor: (underlying) ->
    @underlying = underlying

  position : (vec) =>
    if arguments.length > 0
      @underlying.position = [vec.x, vec.y, vec.z]
    else
      p = @underlying.position
      vec3 p[0], p[1], p[2]

class ActorFactory
  create: (features) =>
    new Actor features.createActor()

actorFactory = new ActorFactory
