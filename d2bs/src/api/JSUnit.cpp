#include "JSUnit.h"

namespace d2bs {
namespace api {

// clang-format off
JSClass JSUnit::js_class_ = {"Unit", 0};

JSPropertySpec JSUnit::js_props_[] = {
  JS_PS_END
};

JSFunctionSpec JSUnit::js_methods_[] = {
  JS_FS_END
};

JSFunctionSpecWithHelp JSUnit::js_functions_[] = {
  JS_FS_HELP_END
};
// clang-format on

bool JSUnit::Define(JSContext* cx, JS::HandleObject obj) {
  if (!JS_InitClass(cx, obj, nullptr, &js_class_, nullptr, 0, js_props_, js_methods_,
                    nullptr, nullptr)) {
    return false;
  }

  if (!JS_DefineFunctionsWithHelp(cx, obj, js_functions_)) {
    return false;
  }

  return true;
}

}  // namespace api
}  // namespace d2bs
