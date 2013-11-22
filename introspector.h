
template <class T1, class T2> void install_accessors(T1 o, T2 object_teml) ;
  /*{
  o->install_accessors(object_teml);
}
  */

template<class SELF, 
         class METHODRET ,
         class METHODRETNATIVE
         > 
void GetNativeAdaptor(
                const char * NAME, 
                const v8::FunctionCallbackInfo<v8::Value>& info, 
                Local<Object> obj ,
                METHODRETNATIVE  (SELF::*method)()const                
                )
{
  SELF  object = (SELF)info;

  // call the method
  METHODRET result = (METHODRET)(object.*method)();

  // it is a native object , so create a generic wrapper for it
  v8::Handle<v8::Value> wrapped_result = wrap_object(result);

  // set the field
  obj->Set(String::NewSymbol(NAME), wrapped_result);
}


/**

   generic accessor helper macros,
   creates a static method that calls the function.
   this is needed to adapt any given function to the needed signature
   NAME = name of the method to call and the suffix of the new method
   TYPE = the resulting field type
   CONV = convert the results to the input to type
   METHOD = method to call   
*/


#define CREATEACCESSOR(NAME,TYPE,CONV,METHOD)    static void GetAccessor ## NAME  (Local<String> property,const v8::PropertyCallbackInfo<Value>& info) { \
    Local<Object> self = info.Holder();                                                                                                             \
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));                                                                        \
    void* ptr_this = wrap->Value();                                                                                                                 \
    info.GetReturnValue().Set(TYPE ((CONV)static_cast<IsolateAdaptor*>(ptr_this)->METHOD)); \
}                                                                                                                                                   \

/**
   a generic installer for accessors
 **/

inline void generic_install (
                      Handle<ObjectTemplate> t, 
                      const char * name, 
                      AccessorGetterCallback cb) {
    Handle<String> object_name(String::New(name));
    t->SetAccessor(object_name, cb);
  }
#define INSTALLACCESSOR(NAME,FNAME)   generic_install(t,NAME,GetAccessor ## FNAME); 
