#include <node.h>
//#include <env.h>
//#include "env-inl.h"
#include <node_object_wrap.h>
using namespace v8;

#include <v8config.h>
#include <v8stdint.h>
#include <v8-debug.h>
#include <v8-defaults.h>
#include <v8-profiler.h>
#include <v8-testing.h>

#include "isolate_wrapper.h"
#include "introspector.h"

template<
  void* (v8::Isolate::* foo)()
 >  void GetT2(
               class v8::Local<v8::String>  property, 
               const class v8::PropertyCallbackInfo<v8::Value>& info               
                                     ) 
{
  // the holder contains the this pointer
  Local<Object> self = info.Holder();

  // the wrap around the this pointer
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));// this object

  void* this_ptr = wrap->Value();
  v8::Isolate * this_obj =this_ptr;
  // call the function pointer via the template param
  void* value = (*this_obj.*foo)();

  info.GetReturnValue().Set(value);
}


class IsolateAdaptor : public v8::Isolate
{
public:

  // got them :
  // inline __attribute__((always_inline)) void SetData(void* data); -- dont support this
  // inline __attribute__((always_inline)) void* GetData();

  // TODO:? 
  // void GetHeapStatistics(HeapStatistics* heap_statistics);
  // heap_statistics->total_heap_size_ =
  // heap_statistics->total_heap_size_executable_ 
  // heap_statistics->total_physical_size_ 
  // heap_statistics->used_heap_size_ 
  // heap_statistics->heap_size_limit_ 

  // TODO:
      // intptr_t AdjustAmountOfExternalAllocatedMemory(intptr_t change_in_bytes);
  // TODO:
      // HeapProfiler* GetHeapProfiler();
  // TODO:
      // CpuProfiler* GetCpuProfiler();
  // TODO:
      // bool InContext();

  // TODO:
      // Local<Context> GetCurrentContext();
  // TODO:
      // Local<Context> GetCallingContext();
  // TODO:
      // Local<Context> GetEnteredContext();

  // set 
      // template<typename T> void SetObjectGroupId(const Persistent<T>& object,       UniqueId id);
      // template<typename T> void SetReferenceFromGroup(UniqueId id, const Persistent<T>& child);
      // template<typename T, typename S>  void SetReference(const Persistent<T>& parent, const Persistent<S>& child);

  // callback types 
      // typedef void (*GCPrologueCallback)(Isolate* isolate, GCType type, GCCallbackFlags flags);
      // typedef void (*GCEpilogueCallback)(Isolate* isolate, GCType type, GCCallbackFlags flags);

  // TODO : add a callback
      // void AddGCPrologueCallback( GCPrologueCallback callback, GCType gc_type_filter = kGCTypeAll);
      // void RemoveGCPrologueCallback(GCPrologueCallback callback);

  // TODO : add a callback
      // void AddGCEpilogueCallback(GCEpilogueCallback callback, GCType gc_type_filter = kGCTypeAll);
      // void RemoveGCEpilogueCallback(GCEpilogueCallback callback);

  // TODO : how to get these values
      // void SetObjectGroupId(internal::Object** object, UniqueId id);
      // void SetReferenceFromGroup(UniqueId id, internal::Object** object);
      // void SetReference(internal::Object** parent, internal::Object** child);


  // create a generic accessor
  CREATEACCESSOR(GetData,Integer::New,long,GetData())


  void install_accessors(Handle<ObjectTemplate> t)
  {
    INSTALLACCESSOR("data2",GetData)
  }

};


template <          
  class METHOD
  > AccessorGetterCallback convert(METHOD m) {
  return m.method_pointer;
}


class HeadProfilerAdaptor : public v8::HeapProfiler {};

template <> 
void install_accessors<
  IsolateAdaptor*, 
  v8::Handle<v8::ObjectTemplate> >(
                                   IsolateAdaptor* o, 
                                   v8::Handle<v8::ObjectTemplate> object_tmpl)
//void install_accessors(T1 o, T2 object_teml) 
{
  o->install_accessors(object_tmpl);
}


