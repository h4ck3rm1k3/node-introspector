
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


template <class METHOD_TYPE, 
          class RETURN_TYPE> 
class TemplateMethodWrapper
{
public:
  typedef RETURN_TYPE return_type;
  typedef METHOD_TYPE method_type;

  TemplateMethodWrapper(METHOD_TYPE method_pointer) : method_pointer(method_pointer)
  {
  }

  METHOD_TYPE method_pointer;
};

template <class U, class T> 
TemplateMethodWrapper<T,U> create_wrapper(T t) 
{
  TemplateMethodWrapper<T,U> wrapper(t);
  return wrapper;
}

template <          
  class RETURN,
  class METHOD, 
  class T> RETURN call(T t, METHOD m) {
  return (t->*m.method_pointer)();
}
