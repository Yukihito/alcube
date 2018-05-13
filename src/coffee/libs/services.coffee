vec3 = (x, y, z) ->
  new THREE.Vector3 parseFloat(x), parseFloat(y), parseFloat(z)

quat = (w, x, y, z) ->
  new THREE.Quaternion x, y, z, w

arrayToVec3 = (raw) ->
  vec3 raw[0], raw[1], raw[2]

vec3ToArray = (value) ->
  [value.x, value.y, value.z]

quatToArray = (value) ->
  [value.w, value.x, value.y, value.z]

arrayToQuat = (raw) ->
  quat raw[0], raw[1], raw[2], raw[3]

rand = () ->
  Math.random()

vec3Zero = vec3(0, 0, 0)
vec3Rand = () ->
  vec3 rand(), rand(), rand()

Texture =
  NONE: 0
  CHECK: 1

InstanceColorType =
  NONE: 0
  MANUAL: 1
  RANDOM: 2
  LINEAR_MOMENTUM: 3

primitiveAccessor = (wrapper, name) ->
  accessor = (value) ->
    if value is undefined
      wrapper.underlying[name]
    else
      wrapper.underlying[name] = value
  wrapper[name] = accessor

vec3Accessor = (wrapper, name) ->
  accessor = (value) ->
    if value is undefined
      arrayToVec3 wrapper.underlying[name]
    else
      wrapper.underlying[name] = vec3ToArray value
  wrapper[name] = accessor

quatAccessor = (wrapper, name) ->
  accessor = (value) ->
    if value is undefined
      arrayToQuat wrapper.underlying[name]
    else
      wrapper.underlying[name] = quatToArray
  wrapper[name] = accessor

class FluidFeatures
  wrap: (underlying) =>
    @underlying = underlying
    primitiveAccessor this, 'mass'
    primitiveAccessor this, 'density'
    primitiveAccessor this, 'stiffness'
    primitiveAccessor this, 'viscosity'

class FluidFeaturesFactory
  wrap: () =>
    @underlying = constructFluidFeaturesFactory()

  create: () =>
    features = new FluidFeatures()
    features.wrap @underlying.create()
    features

class SoftbodyFeatures
  wrap: (underlying) =>
    @underlying = underlying
    primitiveAccessor this, 'elasticity'
    primitiveAccessor this, 'mass'


class SoftbodyFeaturesFactory
  wrap: () =>
    @underlying = constructSoftbodyFeaturesFactory()

  create: () =>
    features = new SoftbodyFeatures()
    features.wrap @underlying.create()
    features

class Spring
  wrap: (underlying) =>
    @underlying = underlying

class SpringFactory
  wrap: () =>
    @underlying = constructSpringFactory()
    primitiveAccessor this, 'k'

  create: (actor0, position0, actor1, position1) =>
    spring = new Spring
    spring.wrap @underlying.create actor0.underlying, vec3ToArray(position0), actor1.underlying, vec3ToArray(position1)
    spring

class Actor
  wrap: (underlying) =>
    @underlying = underlying
    vec3Accessor this, 'position'
    quatAccessor this, 'rotation'
    vec3Accessor this, 'linearMomentum'
    vec3Accessor this, 'angularMomentum'
    vec3Accessor this, 'color'

class ActorFactory
  wrap: () =>
    @underlying = constructActorFactory()

  create: (features, renderingGroup) =>
    actor = new Actor
    actor.wrap(@underlying.create features.underlying, renderingGroup.underlying)
    actor

class RenderingGroup
  wrap: (underlying) =>
    @underlying = underlying
    vec3Accessor this, 'diffuse'
    vec3Accessor this, 'ambient'
    vec3Accessor this, 'specular'
    primitiveAccessor this, 'texture'
    primitiveAccessor this, 'instanceColorType'

  add: (actor) =>
    @underlying.add actor.underlying

class Renderer
  wrap: () =>
    @underlying = constructRenderer()

  createGroup: () =>
    entity = new RenderingGroup()
    entity.wrap @underlying.createGroup()
    entity

class Alcube
  wrap: () =>
    @underlying = constructAlcube()
    primitiveAccessor this, 'actorCount'

fluidFeaturesFactory = new FluidFeaturesFactory()
fluidFeaturesFactory.wrap()

softbodyFeaturesFactory = new SoftbodyFeaturesFactory()
softbodyFeaturesFactory.wrap()

actorFactory = new ActorFactory()
actorFactory.wrap()

springFactory = new SpringFactory()
springFactory.wrap()

renderer = new Renderer()
renderer.wrap()

cube = new Alcube()
cube.wrap()

testbuf = "tes"
