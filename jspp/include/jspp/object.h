#pragma once

#include <jspp/types.h>
#include <jspp/value.h>
#include <jspp/traits.h>

namespace jspp {

struct object : reference<JSObject*> {
  object() {}
  object(JSContext* cx) : reference(cx) {}
  object(JSContext* cx, JSObject* obj) : reference(cx, obj) {}
  object(JSContext* cx, JS::HandleObject obj) : reference(cx, obj) {}
  object(JSContext* cx, JSClass* clasp) : reference(cx, JS_NewObject(cx, clasp)) {}

  void set_private(void* ud) {
    JS_SetPrivate(*this, ud);
  }

  void* get_private() {
    return JS_GetPrivate(*this);
  }
};

}  // namespace jspp
