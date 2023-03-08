#pragma once

#include <jspp/compatibility/js_version.h>

#include <string>
#include <type_traits>

namespace jspp {

struct undefined_t {};
const undefined_t undefined;

enum class type : int {
  undefined = JSTYPE_UNDEFINED,
  null = JSTYPE_NULL,
  boolean = JSTYPE_BOOLEAN,
  number = JSTYPE_NUMBER,
  string = JSTYPE_STRING,
  object = JSTYPE_OBJECT,
  function = JSTYPE_FUNCTION,
  symbol = JSTYPE_SYMBOL,
};

inline std::string type_name(type t) {
  switch (t) {
    case type::null:
      return "null";
    case type::boolean:
      return "boolean";
    case type::number:
      return "number";
    case type::string:
      return "string";
    case type::object:
      return "object";
    case type::function:
      return "function";
    case type::symbol:
      return "symbol";

    // note: undefined and default both return undefined
    case type::undefined:
    default:
      return "undefined";
  }
}

template <typename T>
inline type type_of() {
  if constexpr (std::is_arithmetic_v<T>) {
    return type::number;
  } else {
    return type::undefined;
  }
}

template <>
inline type type_of<undefined_t>() {
  return type::undefined;
}

template <>
inline type type_of<bool>() {
  return type::boolean;
}

template <>
inline type type_of<const char*>() {
  return type::string;
}

template <>
inline type type_of<std::string>() {
  return type::string;
}

}  // namespace jspp
