__kernel void outputActors(
  __global Actor* actors,
  __global ActorState* actorStates
) {
  size_t i = get_global_id(0);
  actors[i] = actorStates[i].constants;
}
