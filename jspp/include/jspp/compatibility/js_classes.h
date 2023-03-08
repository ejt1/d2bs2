#pragma once

namespace jspp {

namespace detail {

// clang-format off
static JSClassOps global_ops = {
    nullptr,                  // JSAddPropertyOp  addProperty
    nullptr,                  // JSDeletePropertyOp delProperty
    nullptr,                  // JSEnumerateOp enumerate
    nullptr,                  // JSNewEnumerateOp newEnumerate
    nullptr,                  // JSResolveOp resolve
    nullptr,                  // JSMayResolveOp mayResolve
    nullptr,                  // JSFinalizeOp finalize
    nullptr,                  // JSNative call
    nullptr,                  // JSHasInstanceOp hasInstance
    nullptr,                  // JSNative construct
    JS_GlobalObjectTraceHook  // JSTraceOp trace
};

static JSClass global_class = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    &global_ops,
};

inline void function_priv_finalize(JSFreeOp*, JSObject* obj) {
  void* voiddata = JS_GetPrivate(obj);
  if (voiddata) {
    delete[] voiddata;
    JS_SetPrivate(obj, nullptr);
  }
}

static JSClassOps function_state_ops = {
    nullptr,                 // JSAddPropertyOp  addProperty
    nullptr,                 // JSDeletePropertyOp delProperty
    nullptr,                 // JSEnumerateOp enumerate
    nullptr,                 // JSNewEnumerateOp newEnumerate
    nullptr,                 // JSResolveOp resolve
    nullptr,                 // JSMayResolveOp mayResolve
    function_priv_finalize,  // JSFinalizeOp finalize
    nullptr,                 // JSNative call
    nullptr,                 // HSHasInstanceOp hasInstance
    nullptr,                 // JSNative construct
    nullptr                  // JSTraceOp trace
};

static JSClass function_state_class = {
    "__jspp__function_state",
    JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
    &function_state_ops,
};

static const char* function_state_prop = "__jspp__function_state_prop";
// clang-format on

}  // namespace detail

}  // namespace jspp
