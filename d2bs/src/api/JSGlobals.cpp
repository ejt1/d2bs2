#include "JSGlobals.h"

#include "core/Settings.h"
#include "game/Game.h"

#include <filesystem>

namespace d2bs {
namespace api {

// clang-format off
JSFunctionSpecWithHelp JSGlobal::js_functions_[] = {
  JS_FN_HELP("include", JSGlobal::Include, 1, 0, "include('foo.js')",
             "  Include script file named by string argument.\n"
             "    Filename is relative to the working directory or\n"
             "    the calling script, in that order."),
  JS_FN_HELP("isIncluded", JSGlobal::isIncluded, 1, 0, "isIncluded('foo.js')",
             "  Return whether file has been included.\n"
             "    This function maybe deprecated in the future!"),
  JS_FN_HELP("print", JSGlobal::Print, 0, 0, "print([exp ...])",
             "  Evaluate and print expression to default logger."),
  JS_FN_HELP("rand", JSGlobal::Rand, 2, 0, "rand(low, high)",
             "  Need description"),
  JS_FN_HELP("say", JSGlobal::Say, 2, 0, "say([exp ...])",
             "  Need description"),

  JS_FS_HELP_END
};
// clang-format on

bool IsAbsolutePath(const JSAutoByteString& filename) {
  const char* pathname = filename.ptr();

  if (pathname[0] == '\\') {
    return true;
  }

  return ((strlen(pathname) > 3 && isalpha(pathname[0]) && pathname[1] == ':' && pathname[2] == '\\'));
}

enum PathResolutionMode { RootRelative, ScriptRelative };

JSString* ResolvePath(JSContext* cx, JS::HandleString filenameStr, PathResolutionMode resolveMode) {
  if (!filenameStr) {
    return nullptr;
  }

  JSAutoByteString filename(cx, filenameStr);
  if (!filename) {
    return nullptr;
  }

  std::filesystem::path abs_path;
  if (resolveMode == RootRelative) {
    abs_path = sSettings->scriptDir;
    // XXX: relative to <working_dir>/libs is a limitation d2bs had, adding it here for now
    abs_path /= "libs";
  } else if (resolveMode == ScriptRelative) {
    JS::AutoFilename scriptFilename;
    if (!JS::DescribeScriptedCaller(cx, &scriptFilename)) {
      return nullptr;
    }

    if (!scriptFilename.get()) {
      return nullptr;
    }

    std::filesystem::path script_path(scriptFilename.get());
    abs_path = script_path.parent_path();
  }

  abs_path /= filename.ptr();
  // Make path into OS preferred so the end result is always the same no matter what separators are used
  abs_path.make_preferred();
  // copying into string so it doesn't fall out of scope when calling JS_NewStringCopyZ
  std::string final_path = abs_path.string();
  return JS_NewStringCopyZ(cx, final_path.c_str());
}

static std::vector<std::string> s_included;

bool JSGlobal::Define(JSContext* cx, JS::HandleObject obj) {
  // define classes, functions etc
  if (!JS_DefineFunctionsWithHelp(cx, obj, js_functions_)) {
    spdlog::error("error defining functions");
    return false;
  }

  return true;
}

bool JSGlobal::Include(JSContext* cx, unsigned argc, JS::Value* vp) {
  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  if (!args.requireAtLeast(cx, "include", 1)) {
    return false;
  }

  JS::RootedString str(cx, JS::ToString(cx, args[0]));
  if (!str) {
    JS_ReportErrorASCII(cx, "{0}: invalid arguments", "include");
    return false;
  }

  // try to resolve relative to working directory
  str = ResolvePath(cx, str, RootRelative);
  if (!str) {
    // try to resolve to executing script directory
    str = ResolvePath(cx, str, ScriptRelative);
    if (!str) {
      JS_ReportErrorASCII(cx, "unable to resolve path");
      return false;
    }
  }

  JSAutoByteString filename(cx, str);
  if (!filename) {
    return false;
  }

  auto it = std::find_if(s_included.begin(), s_included.end(),
                         [&](const std::string& f) { return !_stricmp(filename.ptr(), f.c_str()); });
  if (it == s_included.end()) {
    JS::CompileOptions opts(cx);
    opts.setIntroductionType("d2bs include").setUTF8(true).setIsRunOnce(true).setNoScriptRval(true);
    JS::RootedScript script(cx);
    JS::RootedValue unused(cx);
    if (!JS::Evaluate(cx, opts, filename.ptr(), &unused)) {
      return false;
    }

    s_included.push_back(filename.ptr());
  }

  // XXX: d2bs sets boolean return here, this should be an undefined result instead
  args.rval().setBoolean(true);
  return true;
}

bool JSGlobal::isIncluded(JSContext* cx, unsigned argc, JS::Value* vp) {
  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  if (!args.requireAtLeast(cx, "include", 1)) {
    return false;
  }

  JS::RootedString str(cx, JS::ToString(cx, args[0]));
  if (!str) {
    JS_ReportErrorASCII(cx, "{0}: invalid arguments", "isIncluded");
    return false;
  }

  // try to resolve relative to working directory
  str = ResolvePath(cx, str, RootRelative);
  if (!str) {
    // try to resolve to executing script directory
    str = ResolvePath(cx, str, ScriptRelative);
    if (!str) {
      JS_ReportErrorASCII(cx, "unable to resolve path");
      return false;
    }
  }

  JSAutoByteString filename(cx, str);
  if (!filename) {
    return false;
  }

  auto it = std::find_if(s_included.begin(), s_included.end(),
                         [&](const std::string& f) { return !_stricmp(filename.ptr(), f.c_str()); });
  if (it != s_included.end()) {
    args.rval().setBoolean(true);
  } else {
    args.rval().setBoolean(false);
  }

  return true;
}

bool JSGlobal::Print(JSContext* cx, unsigned argc, JS::Value* vp) {
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

bool JSGlobal::Rand(JSContext* cx, unsigned argc, JS::Value* vp) {
  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  if (!args.requireAtLeast(cx, "rand", 2)) {
    return false;
  }

  if (!args[0].isInt32() || !args[1].isInt32()) {
    JS_ReportErrorASCII(cx, "{0}: invalid arguments", "rand");
    return false;
  }

  static bool seeded = false;
  if (!seeded) {
    srand(GetTickCount());
    seeded = true;
  }

  uint64_t seed = rand();
  if (sGame->State() == GameState::InGame) {
    seed = D2GAME_Rand(D2CLIENT_GetPlayerUnit()->dwSeed);
  } else {
    seed = rand();
  }

  int32_t low = args[0].toInt32();
  int32_t high = args[1].toInt32();
  if (high > low + 1) {
    int32_t rnd = (seed % (static_cast<uint64_t>(high) - low + 1)) + low;
    args.rval().setInt32(rnd);
  } else {
    args.rval().setInt32(high);
  }

  return true;
}

bool JSGlobal::Say(JSContext* cx, unsigned argc, JS::Value* vp) {
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

    sGame->Say(bytes);
  }

  // XXX: d2bs sets true here, should be undefined
  args.rval().setBoolean(true);
  return true;
}

}  // namespace api
}  // namespace d2bs
