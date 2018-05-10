#include "Evaluator.h"
#include "mappings/Settings.h"

namespace alcube::scripting {
  using namespace utils;
  Evaluator::Evaluator(
    alcube::utils::FileUtil* fileUtil,
    const char* programName,
    std::vector<utils::Prototype *> prototypes
  ) {
    this->programName = programName;
    this->prototypes = std::move(prototypes);
    this->fileUtil = fileUtil;
  }

  void Evaluator::evaluate(const char *path) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    v8::Local<v8::String> source =
      v8::String::NewFromUtf8(isolate, (fileUtil->readFile(path)).c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    script->Run(context).IsEmpty();
  }

  void Evaluator::withScope(std::function<void()> f) {
    auto isolate = v8::Isolate::New(createParams);
    {
      v8::Isolate::Scope isolate_scope(isolate);
      v8::HandleScope scope(isolate);
      global = v8::ObjectTemplate::New(isolate);
      registerFunction("print", Evaluator::print);
      initPrototypes();
      context = v8::Context::New(isolate, nullptr, global);
      v8::Context::Scope context_scope(context);
      f();
    }
    isolate->Dispose();
  }

  void Evaluator::initV8() {
    v8::V8::InitializeICUDefaultLocation(programName);
    v8::V8::InitializeExternalStartupData(programName);
    platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
    createParams.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  }

  void Evaluator::finalizeV8() {
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete platform;
    delete createParams.array_buffer_allocator;
  }

  void Evaluator::initPrototypes() {
    for (auto prototype: prototypes) {
      prototype->init();
      prototype->registerConstructor(global);
    }
  }

  void Evaluator::registerFunction(const char *name, v8::FunctionCallback f) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    global->Set(v8::String::NewFromUtf8(isolate, name), v8::FunctionTemplate::New(isolate, f));
  }

  void Evaluator::print(const v8::FunctionCallbackInfo<v8::Value> &args) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    if (args.Length() < 1) {
      args.GetReturnValue().Set(v8::Undefined(isolate));
    }
    v8::Local<v8::Object> arg = args[0]->ToObject();
    v8::String::Utf8Value utf8(arg);
    printf("%s\n", *utf8);
    args.GetReturnValue().Set(v8::Null(isolate));
  }
}