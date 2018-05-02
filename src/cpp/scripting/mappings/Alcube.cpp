#include "Alcube.h"

namespace alcube::scripting::mappings {
  using namespace utils;

  void Alcube::init() {
    Prototype::init();
    define<int, ActorCount>();
    DEFMETHOD(addActor);
    DEFMETHOD(addSpring);
    DEFMETHOD(addRenderingGroup);
  }

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

  void Alcube::addRenderingGroup(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    if (info.Length() < 1) {
      info.GetReturnValue().Set(v8::Undefined(isolate));
      return;
    }
    auto spring = getUnderlying<models::drawing::RenderingGroup>(info[0]);
    self(info)->add(spring);
    info.GetReturnValue().Set(v8::Undefined(isolate));
  }
}
