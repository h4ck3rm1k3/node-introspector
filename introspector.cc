// Copyright 2013 James Michael DuPont. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of James Michael DuPont nor the names of other
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//#define private public 
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

//
#include "introspector.h"
#include "isolate_wrapper.h"


template < class T >  void GetT3(       ) 
{
}


/*
  wrap an external object and install fields to access them
*/
template <class T2> Handle<Object> wrap_object(T2 * o) {

  // create a new template object
  Handle<ObjectTemplate> object_templ = ObjectTemplate::New();
  
  // set the field count to one, this is the external pointer
  object_templ->SetInternalFieldCount(1);

  // install the custom accessor methods into the template
  install_accessors(o, object_templ);

  // create an instance of the template for usag
  Local<Object> obj = object_templ->NewInstance();

  // make a pointer to the external object
  v8::Handle<v8::Value> external_object = External::New(o);
    
  // set the this pointer 
  obj->SetInternalField(0, external_object);

  return obj;
}


template<class SELF, 
         class METHODRETNATIVE
         > 
void set_field_from_method_call(
                const char * NAME, 
                const v8::FunctionCallbackInfo<v8::Value>& info, 
                Local<Object> obj ,
                METHODRETNATIVE  (SELF::*method)()const                
                )
{
  SELF  object = (SELF)info;

  // call the method
  METHODRETNATIVE result = (object.*method)();
  
  // store the result directly
  obj->Set(String::NewSymbol(NAME), result);
}


template<class SELF, 
         class METHODRETNATIVE
         > 
void set_unsigned_field_from_method_call(
                const char * NAME, 
                const v8::FunctionCallbackInfo<v8::Value>& info, 
                Local<Object> obj ,
                METHODRETNATIVE  (SELF::*method)()const 
                )
{
  SELF  object = (SELF)info;

  // call the method
  METHODRETNATIVE result = (object.*method)();
  
  // store the result directly
  obj->Set(String::NewSymbol(NAME), v8::Integer::NewFromUnsigned(result));
}


template<class SELF, class FIELD, class METHODRETNATIVE >  
void GetT(
          Local<String> property,
          const v8::PropertyCallbackInfo<Value>& info,
          METHODRETNATIVE  (SELF::*method)()const 
          ) 
{
  // the holder contains the this pointer
  Local<Object> self = info.Holder();

  // the wrap around the this pointer
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));// this object

  // the this poitner
  void* this_ptr = wrap->Value();

  FIELD value = (FIELD)(this_ptr.*method)();

  info.GetReturnValue().Set(value);
}



template<class T> void CPP(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();

  set_unsigned_field_from_method_call ("IsConstruct",info, obj, &v8::FunctionCallbackInfo<T>::IsConstructCall);
  set_unsigned_field_from_method_call ("Length",info, obj, &v8::FunctionCallbackInfo<T>::Length);
  set_field_from_method_call ("This",info, obj, &v8::FunctionCallbackInfo<T>::This );
  set_field_from_method_call ("Callee",info, obj, &v8::FunctionCallbackInfo<T>::Callee );
  set_field_from_method_call ("Holder",info, obj, &v8::FunctionCallbackInfo<T>::Holder );
  set_field_from_method_call ("Data" , info, obj, &v8::FunctionCallbackInfo<T>::Data );

  // create the isolate wrapper
  GetNativeAdaptor<
    const v8::FunctionCallbackInfo<T>, 
    IsolateAdaptor *, 
    v8::Isolate* > (
                    "Isolate", 
                    info, 
                    obj, 
                    &v8::FunctionCallbackInfo<T>::GetIsolate  
                    );

  // now we put in all the arguments into an array
  for (int i = 0; i < info.Length(); i ++) {
    char buffer[256];
    sprintf(buffer,"Argument_%d",i);
    obj->Set(String::NewSymbol(buffer),  info[i]);  
  }

  info.GetReturnValue().Set(obj);
}

template<class T> void Method(const v8::FunctionCallbackInfo<T>& info) 
{
  HandleScope scope(Isolate::GetCurrent());
  Local<Object> obj = Object::New();
  //obj->Set(String::NewSymbol("javascript"), 
  //           FunctionTemplate::New(JavaScript)->GetFunction());
  obj->Set(String::NewSymbol("cpp"),        
           FunctionTemplate::New(CPP)->GetFunction());            
  info.GetReturnValue().Set(obj);
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("introspector"),
               FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(introspector, init)
