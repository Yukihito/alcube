#include "Alcube.h"

namespace alcube::scripting::mappings {
  using namespace utils;

  void Alcube::init() {
    Prototype::init();
    define<int, ActorCount>();
    DEFMETHOD(gc);
  }

  void Alcube::gc(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    self(info)->gc();
    info.GetReturnValue().Set(v8::Undefined(isolate));
  }
}
