#include <map>
//#include "libplatform/libplatform.h"
//#include "v8.h"
#include "../../utils/FileUtil.h"
#include "../../scripting/mappings/actor/Prototype.h"
#include "../../scripting/Evaluator.h"

/*
class Adder {
  public:
    static void add(const v8::FunctionCallbackInfo<v8::Value>& args);
};

void Adder::add(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  if (args.Length() < 2) {
    args.GetReturnValue().Set(v8::Undefined(isolate));
  }

  int32_t a = args[0]->Int32Value();
  int32_t b = args[1]->Int32Value();
  int32_t sum = a + b;

  args.GetReturnValue().Set(v8::Int32::New(isolate, sum));
}

v8::Local<v8::Value> v8str(v8::Isolate* isolate, const char* str) {
  return v8::Local<v8::Value>::Cast(v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked());
}


void add(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
	if (args.Length() < 2) {
		args.GetReturnValue().Set(v8::Undefined(isolate));
	}

  int32_t a = args[0]->Int32Value();
  int32_t b = args[1]->Int32Value();
	int32_t sum = a + b;

	args.GetReturnValue().Set(v8::Int32::New(isolate, sum));
}

void newObj(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Object> obj = v8::Object::New(isolate);
  obj->Set(v8str(isolate, "hoge"), v8::Number::New(isolate, 5));
  args.GetReturnValue().Set(obj);
}
*/
int main(int argc, char * argv[]) {
  auto fileUtil = new alcube::utils::FileUtil();
  auto actorFactory = new alcube::models::ActorFactory(new alcube::utils::MemoryPool<alcube::models::Actor>(65536));
  auto evaluator = new alcube::scripting::Evaluator(actorFactory, fileUtil, argv[0]);
  evaluator->withScope([](alcube::scripting::Evaluator* e) {
    e->evaluate("../src/js/test.js");
  });

  /*
  auto adder = new Adder();
  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  v8::Platform* platform = v8::platform::CreateDefaultPlatform();
  v8::V8::InitializePlatform(platform);
  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate* isolate = v8::Isolate::New(create_params);
  {
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    global->Set(v8::String::NewFromUtf8(isolate, "add"), v8::FunctionTemplate::New(isolate, adder->add));
    global->Set(v8::String::NewFromUtf8(isolate, "newObj"), v8::FunctionTemplate::New(isolate, newObj));
    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);
    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);
    // Create a string containing the JavaScript source code.
    v8::Local<v8::String> threeJs =
      v8::String::NewFromUtf8(isolate, (fileUtil->readFile("../src/js/three.min.js")).c_str(),
                              v8::NewStringType::kNormal).ToLocalChecked();

    alcube::scripting::mappings::actor::Actor::initObjectTemplate();
    alcube::scripting::mappings::actor::ActorFactory::instance = new alcube::models::ActorFactory(new alcube::models::actor::MemoryPool(65536));
    global->Set(v8::String::NewFromUtf8(isolate, "createActor"), v8::FunctionTemplate::New(isolate, alcube::scripting::mappings::actor::ActorFactory::createFluid));


    v8::Local<v8::String> source =
      v8::String::NewFromUtf8(isolate, (fileUtil->readFile("../src/js/test2.js")).c_str(),
                              v8::NewStringType::kNormal).ToLocalChecked();
    v8::Script::Compile(context, threeJs).ToLocalChecked()->Run(context).ToLocalChecked();
    // Compile the source code.
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    // Run the script to get the result.
    //v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    // Convert the result to an UTF8 string and print it.
    v8::Local<v8::Object> obj = result.As<v8::Object>();
    v8::String::Utf8Value utf8(result);
    v8::Local<v8::External> value = v8::Local<v8::External>::Cast(obj->Get(v8str(isolate, "x")));
    std::cout << value->NumberValue() << std::endl;
    /*
    v8::Local<v8::Value> key = v8str(isolate, "ident");
    auto materialKey = v8str(isolate, "material");
    v8::Local<v8::External> extObj = v8::Local<v8::External>::Cast(obj);
    v8::Local<v8::Array> diffuse = v8::Local<v8::Array>::Cast(v8::Local<v8::External>::Cast(obj->Get(v8str(isolate, "material"))->ToObject()->Get(v8str(isolate, "specular"))));
    for (int i = 0; i < diffuse->Length(); i++) {
      v8::Local<v8::External> field = v8::Local<v8::External>::Cast(diffuse->Get(i));
      if (!field->IsString()) {
        std::cout << field->NumberValue() << std::endl;
      }
    }
     */
  /*
    printf("%s\n", *utf8);
  }
  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::ShutdownPlatform();
  delete platform;
  delete create_params.array_buffer_allocator;
   */

}
