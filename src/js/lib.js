var Actor, ActorFactory, FluidFeatures, FluidFeaturesFactory, Spring, SpringFactory, actorFactory, arrayToQuat, arrayToVec3, fluidFeaturesFactory, numberAccessor, quat, quatAccessor, quatToArray, vec3, vec3Accessor, vec3ToArray;

vec3 = function(x, y, z) {
  return new THREE.Vector3(x, y, z);
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

numberAccessor = function(wrapper, name) {
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
    numberAccessor(this, 'mass');
    numberAccessor(this, 'density');
    numberAccessor(this, 'stiffness');
    return numberAccessor(this, 'viscosity');
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
    return this.underlying = constructSpringFactory();
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
    return vec3Accessor(this, 'angularMomentum');
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

  create(features) {
    var actor;
    actor = new Actor;
    actor.wrap(this.underlying.create(features.underlying));
    return actor;
  }

};

fluidFeaturesFactory = new FluidFeaturesFactory();

fluidFeaturesFactory.wrap();

actorFactory = new ActorFactory();

actorFactory.wrap();
