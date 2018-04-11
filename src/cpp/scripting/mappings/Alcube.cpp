#include "Alcube.h"

namespace alcube::scripting::mappings {
  using namespace alcube::scripting::utils;

  Alcube::Alcube(models::Alcube *underlying) : SingletonPrototype(underlying) {}

  void Alcube::init() {
    utils::Prototype::init();
    define<int, actorCount>();
    defineMethod("addActor", addActor);
    defineMethod("addSpring", addSpring);
  }

  int Alcube::actorCount::get(alcube::models::Alcube *cube) {
    return cube->getActorCount();
  }

  void Alcube::actorCount::set(alcube::models::Alcube *, int) {}

  void Alcube::addActor(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    if (info.Length() < 1) {
      info.GetReturnValue().Set(v8::Undefined(isolate));
      return;
    }
    auto actor = getUnderlying<models::Actor>(info[0]);
    self(info)->add(actor);
    info.GetReturnValue().Set(v8::Undefined(isolate));
  }

  void Alcube::addSpring(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    if (info.Length() < 1) {
      info.GetReturnValue().Set(v8::Undefined(isolate));
      return;
    }
    auto spring = getUnderlying<models::physics::softbody::Spring>(info[0]);
    self(info)->add(spring);
    info.GetReturnValue().Set(v8::Undefined(isolate));
  }
}
