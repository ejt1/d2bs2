#pragma once

#include <jspp/traits.h>
#include <jspp/types.h>

#include <type_traits>

namespace jspp {

namespace detail {

template <typename T>
inline T get_arithmetic(JSContext*, JS::CallArgs* args, std::true_type, int index = -1) {
  return args->get(index).toInt32();
}

template <typename T>
inline T get_arithmetic(JSContext*, JS::CallArgs* args, std::false_type, int index = -1) {
  return static_cast<T>(args->get(index).toNumber());
}

template <typename T>
inline T get_helper(JSContext*, JS::CallArgs*, std::true_type, int index = -1) {
  return 0;
}

template <typename T>
inline T get_helper(JSContext* cx, JS::CallArgs* args, std::false_type, int index = -1) {
  return get_arithmetic<T>(cx, args, std::is_integral<T>{}, index);
}

}  // namespace detail

template <typename T>
inline T get(JSContext* cx, JS::CallArgs* args, int index = -1) {
  return detail::get_helper<T>(cx, args, std::is_class<T>{}, index);
}

template <>
inline bool get<bool>(JSContext*, JS::CallArgs* args, int index) {
  return args->get(index).toBoolean();
}

template <>
inline std::string get<std::string>(JSContext* cx, JS::CallArgs* args, int index) {
  JSAutoRequest r(cx);
  JSString* str = args->get(index).toString();
  std::size_t len = JS_GetStringLength(str);
  std::string buf(len, '\0');
  JS_EncodeStringToBuffer(cx, str, buf.data(), len);
  return buf;
}

template <>
inline const char* get<const char*>(JSContext*, JS::CallArgs*, int) {
  return "";
}

template <typename T>
inline T get_user(JSContext* cx, JSObject* obj) {
  JSAutoRequest ar(cx);
  JS::RootedValue rval(cx);
  JS_GetProperty(cx, JS::RootedObject(cx, obj), detail::function_state_prop, &rval);
  void* voiddata = JS_GetPrivate(rval.toObjectOrNull());
  return *reinterpret_cast<T*>(voiddata);
}

}  // namespace jspp
