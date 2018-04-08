var Actor, ActorFactory, FluidFeatures, actorFactory, vec3;

vec3 = function(x, y, z) {
  return new THREE.Vector3(x, y, z);
};

FluidFeatures = class FluidFeatures {
  constructor() {
    this.createActor = this.createActor.bind(this);
    this.underlying = createActorFactory();
    this.stiffness = 64.0;
    this.density = 0.02;
    this.viscosity = 8.0;
  }

  createActor() {
    return this.underlying.createFluid(this);
  }

};

Actor = class Actor {
  constructor(underlying) {
    this.position = this.position.bind(this);
    this.underlying = underlying;
  }

  position(vec) {
    var p;
    if (arguments.length > 0) {
      return this.underlying.position = [vec.x, vec.y, vec.z];
    } else {
      p = this.underlying.position;
      return vec3(p[0], p[1], p[2]);
    }
  }

};

ActorFactory = class ActorFactory {
  constructor() {
    this.create = this.create.bind(this);
  }

  create(features) {
    return new Actor(features.createActor());
  }

};

actorFactory = new ActorFactory;
