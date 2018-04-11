#ifndef ALCUBE_SCRIPTING_UTILS_H
#define ALCUBE_SCRIPTING_UTILS_H

#include "v8.h"
#include <functional>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <cxxabi.h>
#include <sstream>

namespace alcube::scripting::utils {
  v8::Local<v8::Value> v8str(v8::Isolate* isolate, const char* str);

  class Prototype {
    public:
      v8::Local<v8::ObjectTemplate> objectTemplate;
      virtual void init();
  };

  template <class T>
  class TypedPrototype : public Prototype {
    public:
      explicit TypedPrototype();
      static T* self(const v8::FunctionCallbackInfo<v8::Value>& info);
      static TypedPrototype<T>* instance;
  };

  template <class T>
  T* TypedPrototype<T>::self(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Local<v8::Object> selfObject = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(selfObject->GetInternalField(0));
    void *ptr = wrap->Value();
    return static_cast<T*>(ptr);
  }

  template <class T>
  class SingletonPrototype : public TypedPrototype<T> {
    public:
      explicit SingletonPrototype(T* underlying);
      static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
      static SingletonPrototype<T>* instance;
      T* underlying;
  };

  template <class T>
  void SingletonPrototype<T>::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    auto v8Instance = SingletonPrototype<T>::instance->objectTemplate->NewInstance();
    v8Instance->SetInternalField(0, v8::External::New(isolate, SingletonPrototype<T>::instance->underlying));
    info.GetReturnValue().Set(v8Instance);
  }

  template <class T>
  TypedPrototype<T>::TypedPrototype() {
    TypedPrototype<T>::instance = this;
  }

  template <class T>
  SingletonPrototype<T>::SingletonPrototype(T *underlying) : TypedPrototype<T>::TypedPrototype() {
    this->underlying = underlying;
    SingletonPrototype<T>::instance = this;
  }

  template <class T>
  T* getUnderlying(v8::Local<v8::Value> value);

  template <class T>
  T convertV8ValueTo(v8::Local<v8::Value> value);

  template <class T>
  v8::Handle<v8::Value> convertToV8Value(v8::Isolate* isolate, T raw);

  template <class T, class U>
  static void getter(const v8::PropertyCallbackInfo<v8::Value> &info, std::function<U(T*)> rawGetter);

  template <class T, class U>
  static void setter(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info, std::function<void(T*, U)> rawSetter);

  template <class T, class U, class V>
  class Accessor {
    public:
      static void define(Prototype *prototype);

    protected:
      static U get(T *);
      static void set(T *, U);

    private:
      static void v8Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>&);
      static void v8Setter(v8::Local<v8::String> property, v8::Local<v8::Value>, const v8::PropertyCallbackInfo<void>&);
  };

  template <class T>
  T* getUnderlying(v8::Local<v8::Value> value) {
    v8::Local<v8::Object> obj = value->ToObject();
    auto internalField = obj->GetInternalField(0);
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(internalField);
    void *ptr = wrap->Value();
    return static_cast<T*>(ptr);
  }

  template <class T, class U>
  void getter(const v8::PropertyCallbackInfo<v8::Value> &info, std::function<U(T*)> rawGetter) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    auto instance = static_cast<T*>(ptr);
    auto raw = rawGetter(instance);
    auto array = convertToV8Value<U>(isolate, raw);
    info.GetReturnValue().Set(array);
  }

  template <class T, class U>
  void setter(v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info, std::function<void(T*, U)> rawSetter) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    auto instance = static_cast<T*>(ptr);
    auto raw = convertV8ValueTo<U>(value);
    rawSetter(instance, raw);
  };

  template <class T, class U, class V>
  void Accessor<T, U, V>::define(scripting::utils::Prototype *prototype) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    int stat;
    char *rawName = abi::__cxa_demangle(typeid(V).name(), nullptr, nullptr, &stat);
    std::stringstream name(rawName);
    std::string shortName;
    while (std::getline(name, shortName, ':')) {}
    prototype->objectTemplate->SetAccessor(v8::String::NewFromUtf8(isolate, shortName.c_str()), v8Getter, v8Setter);
    free(rawName);
  }

  template <class T, class U, class V>
  void Accessor<T, U, V>::v8Getter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
    getter<T, U>(info, Accessor<T, U, V>::get);
  }

  template <class T, class U, class V>
  void Accessor<T, U, V>::v8Setter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
    setter<T, U>(value, info, Accessor<T, U, V>::set);
  }
}


namespace alcube::scripting::utils {
  template <class T>
  TypedPrototype<T>* TypedPrototype<T>::instance;

  template <class T>
  SingletonPrototype<T>* SingletonPrototype<T>::instance;
}

#endif //ALCUBE_SCRIPTING_UTILS_H
