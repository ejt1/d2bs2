#include "JSControl.h"

#include "core/Log.h"
#include "game/Control.h"

namespace d2bs {
namespace api {

// clang-format off
JSClassOps JSControl::js_class_ops_ = {
    .finalize = Finalize,
};

JSClass JSControl::js_class_ = {
    .name = "Control", .flags = JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE, .cOps = &js_class_ops_};

JSPropertySpec JSControl::js_props_[] = {
  JS_PS_END
};

JSFunctionSpec JSControl::js_methods_[] = {
  JS_FS_END
};

JSFunctionSpecWithHelp JSControl::js_functions_[] = {
  JS_FN_HELP("getControl", JSControl::getControl, 0, 0, "getControl(type, x, y, sizex, sizey)",
             "  Gets a control given the supplied arguments"),

  JS_FS_HELP_END
};
// clang-format on

JSControl::JSControl(Control* control)
    : type(control->dwType), x(control->dwPosX), y(control->dwPosY), sizex(control->dwSizeX), sizey(control->dwSizeY) {}

bool JSControl::Define(JSContext* cx, JS::HandleObject obj) {
  if (!JS_InitClass(cx, obj, nullptr, &js_class_, nullptr, 0, js_props_, js_methods_, nullptr, nullptr)) {
    return false;
  }

  if (!JS_DefineFunctionsWithHelp(cx, obj, js_functions_)) {
    return false;
  }

  return true;
}

void JSControl::Finalize(JSFreeOp* /*fop*/, JSObject* obj) {
  JSControl* self = static_cast<JSControl*>(JS_GetPrivate(obj));
  if (self) {
    delete self;
  }
}

bool JSControl::getControl(JSContext* cx, unsigned argc, JS::Value* vp) {
  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

  int32_t type = -1, x = -1, y = -1, sizex = -1, sizey = -1;
  int32_t* argproxy[] = {&type, &x, &y, &sizex, &sizey};
  for (uint32_t i = 0; i < args.length(); ++i) {
    if (args[i].isInt32()) {
      *argproxy[i] = args[i].toInt32();
    }
  }

  Control* control = game::FindControl(nullptr, type, -1, x, y, sizex, sizey);
  if (!control) {
    args.rval().setNull();
    return true;
  }

  // XXX: object creation should be wrapped in a helper, maybe in JSBase?
  auto jscontrol = new JSControl(control);
  JS::RootedObject obj(cx, JS_NewObject(cx, &js_class_));
  JS_DefineProperties(cx, obj, js_props_);
  JS_DefineFunctionsWithHelp(cx, obj, js_functions_);
  JS_SetPrivate(obj, jscontrol);

  args.rval().setObject(*obj);
  return true;
}

}  // namespace api
}  // namespace d2bs
