#pragma once

#include <jspp/types.h>

namespace jspp {

template <typename value_t>
class reference {
 public:
  reference() : cx(nullptr) {}

  reference(JSContext* cx) : cx(cx), rooted(cx) {}

  template <typename U>
  reference(JSContext* cx, U&& v) : cx(cx), rooted(cx, v) {}

  reference(const reference& that) {
    *this = that;
  }

  reference& operator=(const reference& that) {
    cx = that.cx;
    rooted.init(cx, that.rooted);
    return *this;
  }

  reference(reference&& that) {
    *this = std::move(that);
  }

  reference& operator=(reference&& that) {
    cx = std::move(that.cx);
    if (rooted.initialized()) {
      rooted.reset();
    }
    rooted.init(cx, std::move(that.rooted));
    return *this;
  }

  operator JS::MutableHandleValue() {
    return &rooted;
  }

  template <typename T>
  operator T() {
    return rooted;
  }

  template <typename T>
  bool is() const {
    auto expected = type_of<T>();
    auto actual = get_type();
    return static_cast<int>(expected) == static_cast<int>(actual);
  }

  type get_type() const {
    JSAutoRequest r(cx);
    return static_cast<type>(JS_TypeOfValue(cx, rooted));
  }

  JSContext* context() {
    return cx;
  }

  JS::MutableHandle<value_t> root() {
    return &rooted;
  }

 protected:
  JSContext* cx;
  JS::PersistentRooted<value_t> rooted;
};

}  // namespace jspp
