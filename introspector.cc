//#define private public 
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

/*
template<typename T>
class FunctionCallbackInfo {
 public:
  inline __attribute__((always_inline)) int Length() const;
  inline __attribute__((always_inline)) Local<Value> operator[](int i) const;
  inline __attribute__((always_inline)) Local<Function> Callee() const;
  inline __attribute__((always_inline)) Local<Object> This() const;
  inline __attribute__((always_inline)) Local<Object> Holder() const;
  inline __attribute__((always_inline)) bool IsConstructCall() const;
  inline __attribute__((always_inline)) Local<Value> Data() const;
  inline __attribute__((always_inline)) Isolate* GetIsolate() const;
  inline __attribute__((always_inline)) ReturnValue<T> GetReturnValue() const;
  static const int kArgsLength = 7;
 protected:
  friend class internal::FunctionCallbackArguments;
  friend class internal::CustomArguments<FunctionCallbackInfo>;
  static const int kHolderIndex = 0;
  static const int kIsolateIndex = 1;
  static const int kReturnValueDefaultValueIndex = 2;
  static const int kReturnValueIndex = 3;
  static const int kDataIndex = 4;
  static const int kCalleeIndex = 5;
  static const int kContextSaveIndex = 6;
  inline __attribute__((always_inline)) FunctionCallbackInfo(internal::Object** implicit_args,
                   internal::Object** values,
                   int length,
                   bool is_construct_call);
  internal::Object** implicit_args_;
  internal::Object** values_;
  int length_;
  bool is_construct_call_;
};

*/
template<class T> void CPP(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  int length = info.Length();
  obj->Set(String::NewSymbol("Length"), 
           v8::Integer::NewFromUnsigned(length));  

  for (int i = 0; i < length; i ++) {
    char buffer[256];
    sprintf(buffer,"Argument_%d",i);
      obj->Set(String::NewSymbol(buffer), 
               info[i]);  
  }

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
