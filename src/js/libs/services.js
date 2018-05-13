var Actor, ActorFactory, Alcube, FluidFeatures, FluidFeaturesFactory, InstanceColorType, Renderer, RenderingGroup, SoftbodyFeatures, SoftbodyFeaturesFactory, Spring, SpringFactory, Texture, actorFactory, arrayToQuat, arrayToVec3, cube, fluidFeaturesFactory, primitiveAccessor, quat, quatAccessor, quatToArray, rand, renderer, softbodyFeaturesFactory, springFactory, testbuf, vec3, vec3Accessor, vec3Rand, vec3ToArray, vec3Zero;

vec3 = function(x, y, z) {
  return new THREE.Vector3(parseFloat(x), parseFloat(y), parseFloat(z));
};

quat = function(w, x, y, z) {
  return new THREE.Quaternion(x, y, z, w);
};

arrayToVec3 = function(raw) {
  return vec3(raw[0], raw[1], raw[2]);
};

vec3ToArray = function(value) {
  return [value.x, value.y, value.z];
};

quatToArray = function(value) {
  return [value.w, value.x, value.y, value.z];
};

arrayToQuat = function(raw) {
  return quat(raw[0], raw[1], raw[2], raw[3]);
};

rand = function() {
  return Math.random();
};

vec3Zero = vec3(0, 0, 0);

vec3Rand = function() {
  return vec3(rand(), rand(), rand());
};

Texture = {
  NONE: 0,
  CHECK: 1
};

InstanceColorType = {
  NONE: 0,
  MANUAL: 1,
  RANDOM: 2,
  LINEAR_MOMENTUM: 3
};

primitiveAccessor = function(wrapper, name) {
  var accessor;
  accessor = function(value) {
    if (value === void 0) {
      return wrapper.underlying[name];
    } else {
      return wrapper.underlying[name] = value;
    }
  };
  return wrapper[name] = accessor;
};

vec3Accessor = function(wrapper, name) {
  var accessor;
  accessor = function(value) {
    if (value === void 0) {
      return arrayToVec3(wrapper.underlying[name]);
    } else {
      return wrapper.underlying[name] = vec3ToArray(value);
    }
  };
  return wrapper[name] = accessor;
};

quatAccessor = function(wrapper, name) {
  var accessor;
  accessor = function(value) {
    if (value === void 0) {
      return arrayToQuat(wrapper.underlying[name]);
    } else {
      return wrapper.underlying[name] = quatToArray;
    }
  };
  return wrapper[name] = accessor;
};

FluidFeatures = class FluidFeatures {
  constructor() {
    this.wrap = this.wrap.bind(this);
  }

  wrap(underlying) {
    this.underlying = underlying;
    primitiveAccessor(this, 'mass');
    primitiveAccessor(this, 'density');
    primitiveAccessor(this, 'stiffness');
    return primitiveAccessor(this, 'viscosity');
  }

};

FluidFeaturesFactory = class FluidFeaturesFactory {
  constructor() {
    this.wrap = this.wrap.bind(this);
    this.create = this.create.bind(this);
  }

  wrap() {
    return this.underlying = constructFluidFeaturesFactory();
  }

  create() {
    var features;
    features = new FluidFeatures();
    features.wrap(this.underlying.create());
    return features;
  }

};

SoftbodyFeatures = class SoftbodyFeatures {
  constructor() {
    this.wrap = this.wrap.bind(this);
  }

  wrap(underlying) {
    this.underlying = underlying;
    primitiveAccessor(this, 'elasticity');
    return primitiveAccessor(this, 'mass');
  }

};

SoftbodyFeaturesFactory = class SoftbodyFeaturesFactory {
  constructor() {
    this.wrap = this.wrap.bind(this);
    this.create = this.create.bind(this);
  }

  wrap() {
    return this.underlying = constructSoftbodyFeaturesFactory();
  }

  create() {
    var features;
    features = new SoftbodyFeatures();
    features.wrap(this.underlying.create());
    return features;
  }

};

Spring = class Spring {
  constructor() {
    this.wrap = this.wrap.bind(this);
  }

  wrap(underlying) {
    return this.underlying = underlying;
  }

};

SpringFactory = class SpringFactory {
  constructor() {
    this.wrap = this.wrap.bind(this);
    this.create = this.create.bind(this);
  }

  wrap() {
    this.underlying = constructSpringFactory();
    return primitiveAccessor(this, 'k');
  }

  create(actor0, position0, actor1, position1) {
    var spring;
    spring = new Spring;
    spring.wrap(this.underlying.create(actor0.underlying, vec3ToArray(position0), actor1.underlying, vec3ToArray(position1)));
    return spring;
  }

};

Actor = class Actor {
  constructor() {
    this.wrap = this.wrap.bind(this);
  }

  wrap(underlying) {
    this.underlying = underlying;
    vec3Accessor(this, 'position');
    quatAccessor(this, 'rotation');
    vec3Accessor(this, 'linearMomentum');
    vec3Accessor(this, 'angularMomentum');
    return vec3Accessor(this, 'color');
  }

};

ActorFactory = class ActorFactory {
  constructor() {
    this.wrap = this.wrap.bind(this);
    this.create = this.create.bind(this);
  }

  wrap() {
    return this.underlying = constructActorFactory();
  }

  create(features, renderingGroup) {
    var actor;
    actor = new Actor;
    actor.wrap(this.underlying.create(features.underlying, renderingGroup.underlying));
    return actor;
  }

};

RenderingGroup = class RenderingGroup {
  constructor() {
    this.wrap = this.wrap.bind(this);
    this.add = this.add.bind(this);
  }

  wrap(underlying) {
    this.underlying = underlying;
    vec3Accessor(this, 'diffuse');
    vec3Accessor(this, 'ambient');
    vec3Accessor(this, 'specular');
    primitiveAccessor(this, 'texture');
    return primitiveAccessor(this, 'instanceColorType');
  }

  add(actor) {
    return this.underlying.add(actor.underlying);
  }

};

Renderer = class Renderer {
  constructor() {
    this.wrap = this.wrap.bind(this);
    this.createGroup = this.createGroup.bind(this);
  }

  wrap() {
    return this.underlying = constructRenderer();
  }

  createGroup() {
    var entity;
    entity = new RenderingGroup();
    entity.wrap(this.underlying.createGroup());
    return entity;
  }

};

Alcube = class Alcube {
  constructor() {
    this.wrap = this.wrap.bind(this);
  }

  wrap() {
    this.underlying = constructAlcube();
    return primitiveAccessor(this, 'actorCount');
  }

};

fluidFeaturesFactory = new FluidFeaturesFactory();

fluidFeaturesFactory.wrap();

softbodyFeaturesFactory = new SoftbodyFeaturesFactory();

softbodyFeaturesFactory.wrap();

actorFactory = new ActorFactory();

actorFactory.wrap();

springFactory = new SpringFactory();

springFactory.wrap();

renderer = new Renderer();

renderer.wrap();

cube = new Alcube();

cube.wrap();

testbuf = "tes";
