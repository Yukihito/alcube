#include "Evaluator.h"

namespace alcube::scripting {
  using namespace utils;
  Evaluator::Evaluator(
    alcube::models::ActorFactory *actorFactory,
    alcube::utils::FileUtil* fileUtil,
    const char* programName
  ) {
    this->programName = programName;
    prototypes = {};
    prototypes.push_back(new mappings::Actor::Prototype());
    prototypes.push_back(new mappings::ActorFactory::Prototype(actorFactory));
    this->fileUtil = fileUtil;
  }

  void Evaluator::evaluate(const char *path) {
    v8::HandleScope scope(isolate);
    v8::Local<v8::String> source =
      v8::String::NewFromUtf8(isolate, (fileUtil->readFile(path)).c_str(),
                              v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    v8::String::Utf8Value utf8(result);
    printf("%s\n", *utf8);
  }

  void Evaluator::withScope(std::function<void(Evaluator*)> f) {
    initV8();
    {
      v8::Isolate::Scope isolate_scope(isolate);
      v8::HandleScope scope(isolate);
      global = v8::ObjectTemplate::New(isolate);
      initTemplates();
      registerFunctions();
      context = v8::Context::New(isolate, nullptr, global);
      v8::Context::Scope context_scope(context);
      loadLibs();
      f(this);
    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete platform;
    delete createParams.array_buffer_allocator;
  }

  void Evaluator::initV8() {
    v8::V8::InitializeICUDefaultLocation(programName);
    v8::V8::InitializeExternalStartupData(programName);
    platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
    createParams.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(createParams);
  }

  void Evaluator::loadLibs() {
    loadLib("../src/js/three.min.js");
    loadLib("../src/js/lib.js");
  }

  void Evaluator::loadLib(const char *filePath) {
    v8::Local<v8::String> threeJs =
      v8::String::NewFromUtf8(isolate, (fileUtil->readFile(filePath)).c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Script::Compile(context, threeJs).ToLocalChecked()->Run(context).ToLocalChecked();
  }

  void Evaluator::initTemplates() {
    for (auto prototype : prototypes) {
      prototype->init();
    }
  }

  void Evaluator::registerFunctions() {
    registerFunction("createActorFactory", mappings::ActorFactory::Prototype::constructor);
    registerFunction("print", Evaluator::print);
  }

  void Evaluator::registerFunction(const char *name, v8::FunctionCallback f) {
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