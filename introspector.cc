//#define private public 
#include <node.h>
//#include <env.h>
//#include "env-inl.h"
#include <node_object_wrap.h>
using namespace v8;
class IsolateAdaptor : public v8::Isolate
{

public:

static void GetDataAccessor(Local<String> property,
                                const v8::PropertyCallbackInfo<Value>& info
                                ) {
Local<Object> self = info.Holder();
Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
void* ptr = wrap->Value();
long value =  (long)static_cast<IsolateAdaptor*>(ptr)->GetData();
//returnval = 
info.GetReturnValue().Set(Integer::New(value));
  }


void install_accessors(Handle<ObjectTemplate> t)
{
Handle<String> object_name(String::New("data"));


AccessorGetterCallback cb=GetDataAccessor;
t->SetAccessor(object_name, cb);
/*
class Isolate {
 public:
  class Scope {
   public:
    explicit Scope(Isolate* isolate) : isolate_(isolate) {
      isolate->Enter();
    }
    ~Scope() { isolate_->Exit(); }
   private:
    Isolate* const isolate_;
    Scope(const Scope&);
    Scope& operator=(const Scope&);
  };
  static Isolate* New();
  static Isolate* GetCurrent();
  void Enter();
  void Exit();
  void Dispose();
  inline __attribute__((always_inline)) void SetData(void* data);
  inline __attribute__((always_inline)) void* GetData();
  void GetHeapStatistics(HeapStatistics* heap_statistics);

  intptr_t AdjustAmountOfExternalAllocatedMemory(intptr_t change_in_bytes);
  HeapProfiler* GetHeapProfiler();
  CpuProfiler* GetCpuProfiler();
  bool InContext();
  Local<Context> GetCurrentContext();
  Local<Context> GetCallingContext();
  Local<Context> GetEnteredContext();
  Local<Value> ThrowException(Local<Value> exception);
  template<typename T> void SetObjectGroupId(const Persistent<T>& object,                                             UniqueId id);

  template<typename T> void SetReferenceFromGroup(UniqueId id,                                                  const Persistent<T>& child);
  template<typename T, typename S>  void SetReference(const Persistent<T>& parent, const Persistent<S>& child);
  typedef void (*GCPrologueCallback)(Isolate* isolate,
                                     GCType type,
                                     GCCallbackFlags flags);
  typedef void (*GCEpilogueCallback)(Isolate* isolate,
                                     GCType type,
                                     GCCallbackFlags flags);
  void AddGCPrologueCallback(
      GCPrologueCallback callback, GCType gc_type_filter = kGCTypeAll);
  void RemoveGCPrologueCallback(GCPrologueCallback callback);
  void AddGCEpilogueCallback(
      GCEpilogueCallback callback, GCType gc_type_filter = kGCTypeAll);


  void RemoveGCEpilogueCallback(GCEpilogueCallback callback);
 private:
  Isolate();
  Isolate(const Isolate&);
  ~Isolate();
  Isolate& operator=(const Isolate&);
  void* operator new(size_t size);
  void operator delete(void*, size_t);
  void SetObjectGroupId(internal::Object** object, UniqueId id);
  void SetReferenceFromGroup(UniqueId id, internal::Object** object);
  void SetReference(internal::Object** parent, internal::Object** child);
};

*/
}

};

template <class T2> Handle<Object> wrap_object(T2 * o) {
  Handle<ObjectTemplate> object_templ = ObjectTemplate::New();
  object_templ->SetInternalFieldCount(1);
  o->install_accessors(object_templ);

  Local<Object> obj = object_templ->NewInstance();
  obj->SetInternalField(0, External::New(o));
  return obj;
}


/*
this class is not used yet, see the wrap_object above for wrapping an existing object
*/
template <class T2> 
class TWrap : public node::ObjectWrap
{
public:

static void Init() {

/*
install the new function
*/

Isolate* isolate = Isolate::GetCurrent();
HandleScope scope(isolate);
// Prepare constructor template
Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
tpl->SetClassName(String::NewSymbol(T2::getName()));
tpl->InstanceTemplate()->SetInternalFieldCount(T2::getFieldCount());
constructor.Reset(isolate, tpl->GetFunction());
}

static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
/*
  
 */
Isolate* isolate = Isolate::GetCurrent();
HandleScope scope(isolate);
const unsigned argc = 1;
Handle<Value> argv[argc] = { info[0] };
Local<Object> instance = Local<Function>::New(isolate, constructor)->NewInstance(argc, argv);
info.GetReturnValue().Set(instance);

}

T2 Val() const { return val_; }

private:
TWrap() {}
~TWrap() {}

static v8::Persistent<v8::Function> constructor;

template<class T> static void New(const v8::FunctionCallbackInfo<T>& info){
/*
  extract value from info and wrap it
*/
Isolate* isolate = Isolate::GetCurrent();
HandleScope scope(isolate);
TWrap<T2>* obj = new TWrap<T2>();
obj->val_ = info[0]->IsUndefined() ? T2::DefaultValue() : T2::ExtractValue(info[0]);
obj->Wrap(info.This());
}

T2 val_;
};

typedef TWrap<IsolateAdaptor> IsolateWrap;

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


  Local<Function> callee     = info.Callee();
  Local<Object>   obj_this   = info.This();
  Local<Object>   holder     = info.Holder();
  bool            _iscontruct = info.IsConstructCall();
  Local<Value>    iscontruct  = v8::Integer::NewFromUnsigned(_iscontruct);
  Local<Value>    data       = info.Data();

  // an adaptor 
  IsolateAdaptor*   isolate_val = (IsolateAdaptor*)info.GetIsolate();
  
  //IsolateWrap isolate_wrapper;

  //ReturnValue<T>  returnval  = info.GetReturnValue();


  obj->Set(String::NewSymbol("Length"), 
           v8::Integer::NewFromUnsigned(length));  
  obj->Set(String::NewSymbol("Callee"), callee);
  obj->Set(String::NewSymbol("This"), obj_this);
  obj->Set(String::NewSymbol("Holder"), holder);
  obj->Set(String::NewSymbol("IsConstruct"), iscontruct);
  obj->Set(String::NewSymbol("Data"), data);
  obj->Set(String::NewSymbol("Isolate"), wrap_object(isolate_val));
   //obj->Set(String::NewSymbol("Return"), returnval);

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
