var Actor, Material, actor, vec, vec3;

Material = class Material {
  constructor(diffuse, ambient, specular) {
    this.diffuse = diffuse;
    this.ambient = ambient;
    this.specular = specular;
  }

};

Actor = class Actor {
  constructor(ident, position, material) {
    this.ident = ident;
    this.position = position;
    this.material = material;
  }

};

vec3 = function(x, y, z) {
  return new THREE.Vector3(x, y, z);
};

actor = new Actor(1, [0, 1.2, 3.14], new Material([1.0, 0.0, 0.0], [0.0, 2.0, 0.0], [0.0, 0.0, 3.0]));

// actor
vec = vec3(newObj()['hoge'], 2.0, 3.0);

vec;
