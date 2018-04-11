#include "utils.h"

namespace alcube::scripting::utils {
  v8::Local<v8::Value> v8str(v8::Isolate* isolate, const char* str) {
    return v8::Local<v8::Value>::Cast(v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked());
  }

  void Prototype::init() {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    objectTemplate = v8::ObjectTemplate::New(isolate);
    objectTemplate->SetInternalFieldCount(1);
  }

  void Prototype::defineMethod(const char *name, v8::FunctionCallback f) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    objectTemplate->Set(
      v8::String::NewFromUtf8(isolate, name),
      v8::FunctionTemplate::New(isolate, f)
    );
  }

  // float
  template <>
  float convertV8ValueTo(v8::Local<v8::Value> value) {
    return (float)value->NumberValue();
  }

  template <>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, float raw) {
    return v8::Number::New(isolate, raw);
  }

  // int
  template <>
  int convertV8ValueTo(v8::Local<v8::Value> value) {
    return value->Int32Value();
  }

  template <>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, int raw) {
    return v8::Int32::New(isolate, raw);
  }

  // vec3
  template<>
  glm::vec3 convertV8ValueTo(v8::Local<v8::Value> value) {
    v8::Local<v8::Array> array = value.As<v8::Array>();
    return glm::vec3(
      (float)array->Get(0)->NumberValue(),
      (float)array->Get(1)->NumberValue(),
      (float)array->Get(2)->NumberValue()
    );
  }

  template<>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, glm::vec3 raw) {
    int length = 3;
    v8::Handle<v8::Array> array = v8::Array::New(isolate, length);
    for (uint32_t i = 0; i < length; i++) {
      array->Set(i, v8::Number::New(isolate, raw[i]));
    }
    return array;
  }

  // quat
  template<>
  glm::quat convertV8ValueTo(v8::Local<v8::Value> value) {
    v8::Local<v8::Array> array = value.As<v8::Array>();
    return glm::quat(
      (float)array->Get(0)->NumberValue(),
      (float)array->Get(1)->NumberValue(),
      (float)array->Get(2)->NumberValue(),
      (float)array->Get(3)->NumberValue()
    );
  }

  template<>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, glm::quat raw) {
    int length = 4;
    v8::Handle<v8::Array> array = v8::Array::New(isolate, length);
    array->Set(0, v8::Number::New(isolate, raw.w));
    array->Set(1, v8::Number::New(isolate, raw.x));
    array->Set(2, v8::Number::New(isolate, raw.y));
    array->Set(3, v8::Number::New(isolate, raw.z));
    return array;
  }
}