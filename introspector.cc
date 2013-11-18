#include <node.h>
//#include <env.h>
//#include "env-inl.h"

using namespace v8;


//  Local<Object> global = env->context()->Global();

template<class T> void JavaScriptGlobal(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  info.GetReturnValue().Set(obj);
}

template<class T> void JavaScriptContext(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("global"), 
           FunctionTemplate::New(                                 
                                 JavaScriptGlobal
                                )->GetFunction());
  info.GetReturnValue().Set(obj);
}

template<class T> void JavaScriptEnv(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();


  //Environment* env = env();//Environment::GetCurrent(context);

  obj->Set(String::NewSymbol("context"), FunctionTemplate::New(JavaScriptContext)->GetFunction());
  info.GetReturnValue().Set(obj);
}

template<class T> void JavaScript(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  // TODO : this is basically the process object... we need to check if it can be leveraged.
  obj->Set(String::NewSymbol("env"), 
           FunctionTemplate::New(JavaScriptEnv)->GetFunction());

  info.GetReturnValue().Set(obj);
}

template<class T> void CPP(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  info.GetReturnValue().Set(obj);
}

template<class T> void Method(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("javascript"), 
           FunctionTemplate::New(JavaScript)->GetFunction());
  obj->Set(String::NewSymbol("cpp"),        
           FunctionTemplate::New(CPP)->GetFunction());            
  info.GetReturnValue().Set(obj);
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("introspector"),
               FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(introspector, init)
