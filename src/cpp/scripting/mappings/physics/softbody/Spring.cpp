#include "Spring.h"

namespace alcube::scripting::mappings::physics::softbody {
  using namespace utils;

  SpringFactory::SpringFactory(models::physics::softbody::SpringFactory *underlying) : utils::SingletonPrototype<models::physics::softbody::SpringFactory>(underlying) {}

  void SpringFactory::init() {
    Prototype::init();
    define<float, K>();
    DEFMETHOD(create);
  }

  void SpringFactory::create(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    if (info.Length() < 4) {
      info.GetReturnValue().Set(v8::Undefined(isolate));
    }
    auto factory = self(info);
    auto actor0 = getUnderlying<models::Actor>(info[0]);
    auto pos0 = convertV8ValueTo<glm::vec3>(info[1]);
    auto actor1 = getUnderlying<models::Actor>(info[2]);
    auto pos1 = convertV8ValueTo<glm::vec3>(info[3]);
    auto spring = factory->create(actor0, pos0, actor1, pos1);
    auto wrapper = Spring::instance->objectTemplate->NewInstance();
    wrapper->SetInternalField(0, v8::External::New(isolate, spring));
    info.GetReturnValue().Set(wrapper);
  }
}
