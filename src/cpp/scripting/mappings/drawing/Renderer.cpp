#include "Renderer.h"

namespace alcube::scripting::mappings::drawing {
  using namespace utils;

  void Renderer::init() {
    Prototype::init();
    DEFMETHOD(createGroup);
  }

  void Renderer::createGroup(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    auto underlying = self(info)->createGroup();
    auto entity = RenderingGroup::instance->objectTemplate->NewInstance();
    entity->SetInternalField(0, v8::External::New(isolate, underlying));
    info.GetReturnValue().Set(entity);
  }
}
