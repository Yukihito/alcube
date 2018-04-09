var Actor, ActorFactory, FluidFeatures, FluidFeaturesFactory, actorFactory, fluidFeaturesFactory, numberAccessor, quat, quatAccessor, vec3, vec3Accessor;

vec3 = function(x, y, z) {
  return new THREE.Vector3(x, y, z);
};

quat = function(w, x, y, z) {
  return new THREE.Quaternion(x, y, z, w);
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
    var raw;
    if (value === void 0) {
      raw = wrapper.underlying[name];
      return vec3(raw[0], raw[1], raw[2]);
    } else {
      return wrapper.underlying[name] = [value.x, value.y, value.z];
    }
  };
  return wrapper[name] = accessor;
};

quatAccessor = function(wrapper, name) {
  var accessor;
  accessor = function(value) {
    var raw;
    if (value === void 0) {
      raw = wrapper.underlying[name];
      return quat(raw[0], raw[1], raw[2], raw[3]);
    } else {
      return wrapper.underlying[name] = [value.w, value.x, value.y, value.z];
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
