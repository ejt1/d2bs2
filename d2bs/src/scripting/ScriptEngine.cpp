#include "ScriptEngine.h"

#include "api/All.h"
#include "core/Settings.h"

#include <fstream>

namespace d2bs {

void WarningReporter(JSContext*, JSErrorReport* report) {
  spdlog::error("{}:{} {}", report->filename, report->lineno, report->message().c_str());
}

class AutoReportException {
 public:
  AutoReportException(JSContext* cx) : cx(cx) {}
  ~AutoReportException() {
    if (!JS_IsExceptionPending(cx)) {
      return;
    }

    JS::RootedValue ex(cx);
    JS_GetPendingException(cx, &ex);
    JS_ClearPendingException(cx);

    js::ErrorReport report(cx);
    if (!report.init(cx, ex, js::ErrorReport::WithSideEffects)) {
      spdlog::error("out of memory initializing ErrorReport");
      JS_ClearPendingException(cx);
      return;
    }

    // XXX: Should be a function that both warning reporter and exception reporter
    // calls, dont call the warning reporter!
    WarningReporter(cx, report.report());
  }

 private:
  JSContext* cx;
};

ScriptEngine::ScriptEngine() {}

ScriptEngine::~ScriptEngine() {}

bool ScriptEngine::Init() {
  if (init_) {
    return true;
  }
  init_ = true;

  // init stuff
  cx_ = jspp::context(sSettings->dwMemUsage);
  JS_SetContextPrivate(cx_, new ScriptContext);

  {
    JSAutoRequest request(cx_);

    if (!api::DefineAll(cx_, cx_.global())) {
      spdlog::error("error defining api");
      return false;
    }
  }

  // load script
  std::filesystem::path script_file;
  auto profile = sSettings->ActiveProfile();
  if (!profile.starterScript.empty()) {
    script_file = sSettings->scriptDir / profile.starterScript;
  } else {
    script_file = sSettings->scriptDir / sSettings->szStarter;
  }

  if (!std::filesystem::exists(script_file)) {
    spdlog::error("script {} does not exist", script_file.string());
    return false;
  }
  std::string filename = script_file.string();

  std::ifstream f(script_file);
  std::stringstream code;
  code << f.rdbuf();

  {
    JSAutoRequest request(cx_);

    JS::CompileOptions opts(cx_);
    opts.setUTF8(true).setFileAndLine(filename.c_str(), 1);

    JS::RootedValue rval(cx_);
    AutoReportException are(cx_);
    if (!JS::Evaluate(cx_, opts, code.str().c_str(), code.str().length(), &rval)) {
      // error is logged by AutoReportException
      return false;
    }
  }

  return true;
}

void ScriptEngine::Shutdown() {
  if (!init_) {
    return;
  }
  init_ = false;

  // shutdown stuff
  ScriptContext* sc = ScriptContext::GetFromJS(cx_);
  if (sc) {
    delete sc;
  }

  cx_ = {};
}

void ScriptEngine::Update() {
  js::RunJobs(cx_);
}

}  // namespace d2bs
