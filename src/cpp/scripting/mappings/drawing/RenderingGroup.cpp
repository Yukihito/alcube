#include "RenderingGroup.h"

namespace alcube::scripting::mappings::drawing {
  using namespace utils;

  void RenderingGroup::init() {
    Prototype::init();
    define<glm::vec3, Diffuse>();
    define<glm::vec3, Ambient>();
    define<glm::vec3, Specular>();
    define<models::drawing::Texture, Texture>();
    define<models::drawing::InstanceColorType, InstanceColorType>();
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
