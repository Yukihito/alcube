#include "Renderer.h"

namespace alcube::scripting::mappings::drawing {
  using namespace utils;

  void Renderer::init() {
    Prototype::init();
    define<glm::vec3, Diffuse>();
    define<glm::vec3, Ambient>();
    define<glm::vec3, Specular>();
    define<models::drawing::Texture, Texture>();
    define<models::drawing::InstanceColorType, InstanceColorType>();
    DEFMETHOD(setUpResources);
  }

  void Renderer::setUpResources(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    self(info)->setUpResources();
    info.GetReturnValue().Set(v8::Null(isolate));
  }

  void RendererFactory::init() {
    Prototype::init();
    DEFMETHOD(create);
  }

  void RendererFactory::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    auto underlying = self(info)->create();
    auto renderer = Renderer::instance->objectTemplate->NewInstance();
    renderer->SetInternalField(0, v8::External::New(isolate, underlying));
    info.GetReturnValue().Set(renderer);
  }
}

namespace alcube::scripting::utils {
  using namespace alcube::models::drawing;
  // Texture
  template <>
  Texture convertV8ValueTo(v8::Local<v8::Value> value) {
    return (Texture)value->Int32Value();
  }

  template <>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, Texture raw) {
    return v8::Int32::New(isolate, (int)raw);
  }

  // InstanceColorType
  template <>
  InstanceColorType convertV8ValueTo(v8::Local<v8::Value> value) {
    return (InstanceColorType)value->Int32Value();
  }

  template <>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, InstanceColorType raw) {
    return v8::Int32::New(isolate, (int)raw);
  }
}
