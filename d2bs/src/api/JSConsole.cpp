#include "JSConsole.h"

#include "core/Log.h"

namespace d2bs {
namespace api {

// clang-format off
JSClass JSConsole::js_class_ = {"Console", 0};

JSPropertySpec JSConsole::js_props_[] = {
  JS_PS_END
};

JSFunctionSpec JSConsole::js_methods_[] = {
  JS_FS_END
};

JSFunctionSpecWithHelp JSConsole::js_functions_[] = {
  JS_FN_HELP("log", JSConsole::Log, 0, 0, "log([exp ...])",
             "  Evaluate and print expressions to in-game console."),

  JS_FS_HELP_END
};
// clang-format on

bool JSConsole::Define(JSContext* cx, JS::HandleObject global) {
  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  return obj && JS_DefineFunctionsWithHelp(cx, obj, js_functions_) && JS_DefineProperty(cx, global, "console", obj, 0);
}

// XXX: Because in-game console is not added yet, just copy-paste Print function
bool JSConsole::Log(JSContext* cx, unsigned argc, JS::Value* vp) {
  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

  std::stringstream msg;
  for (uint32_t i = 0; i < args.length(); ++i) {
    JS::RootedString str(cx, JS::ToString(cx, args[i]));
    if (!str) {
      return false;
    }

    char* bytes = JS_EncodeStringToUTF8(cx, str);
    if (!bytes) {
      return false;
    }

    msg << (i ? " " : "") << bytes;
  }
  spdlog::info("{}", msg.str());

  args.rval().setUndefined();
  return true;
}

}  // namespace api
}  // namespace d2bs
