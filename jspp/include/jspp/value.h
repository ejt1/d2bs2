#pragma once

#include <jspp/reference.h>

namespace jspp {

class value : public reference<JS::Value> {
 public:
  value(JSContext* cx) : reference(cx), cx(cx) {}

  template <typename U>
  value(JSContext* cx, U&& u) : reference(cx, u), cx(cx) {}

 private:
  JSContext* cx;
};

}  // namespace jspp
