vec3 = (x, y, z) ->
  new THREE.Vector3 x, y, z

quat = (w, x, y, z) ->
  new THREE.Quaternion x, y, z, w

numberAccessor = (wrapper, name) ->
  accessor = (value) ->
    if value is undefined
      wrapper.underlying[name]
    else
      wrapper.underlying[name] = value
  wrapper[name] = accessor

vec3Accessor = (wrapper, name) ->
  accessor = (value) ->
    if value is undefined
      raw = wrapper.underlying[name]
      vec3 raw[0], raw[1], raw[2]
    else
      wrapper.underlying[name] = [value.x, value.y, value.z]
  wrapper[name] = accessor

quatAccessor = (wrapper, name) ->
  accessor = (value) ->
    if value is undefined
      raw = wrapper.underlying[name]
      quat raw[0], raw[1], raw[2], raw[3]
    else
      wrapper.underlying[name] = [value.w, value.x, value.y, value.z]
  wrapper[name] = accessor

class FluidFeatures
  wrap: (underlying) =>
    @underlying = underlying
    numberAccessor this, 'mass'
    numberAccessor this, 'density'
    numberAccessor this, 'stiffness'
    numberAccessor this, 'viscosity'

class FluidFeaturesFactory
  wrap: () =>
    @underlying = constructFluidFeaturesFactory()

  create: () =>
    features = new FluidFeatures()
    features.wrap @underlying.create()
    features

class Actor
  wrap: (underlying) =>
    @underlying = underlying
    vec3Accessor this, 'position'
    quatAccessor this, 'rotation'
    vec3Accessor this, 'linearMomentum'
    vec3Accessor this, 'angularMomentum'

class ActorFactory
  wrap: () =>
    @underlying = constructActorFactory()

  create: (features) =>
    actor = new Actor
    actor.wrap @underlying.create features.underlying
    actor

fluidFeaturesFactory = new FluidFeaturesFactory()
fluidFeaturesFactory.wrap()

actorFactory = new ActorFactory()
actorFactory.wrap()
