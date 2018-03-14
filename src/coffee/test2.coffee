class Material
  constructor: (@diffuse, @ambient, @specular) ->

class Actor
  constructor: (@ident, @position, @material) ->

vec3 = (x, y, z) ->
  new THREE.Vector3(x, y, z);

actor = new Actor(1, [0, 1.2, 3.14], new Material([1.0, 0.0, 0.0], [0.0, 2.0, 0.0], [0.0, 0.0, 3.0]))

# actor

vec = vec3(newObj()['hoge'], 2.0, 3.0);
vec