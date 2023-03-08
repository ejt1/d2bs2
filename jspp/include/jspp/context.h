#pragma once

#include <jspp/object.h>
#include <jspp/types.h>

#include <memory>

namespace jspp {

namespace detail {
struct context_deleter {
  void operator()(JSContext* cx) {
    JS_DestroyContext(cx);
  }
};
}  // namespace detail

class context : private std::unique_ptr<JSContext, detail::context_deleter> {
  typedef std::unique_ptr<JSContext, detail::context_deleter> unique_base;

 public:
  context() : unique_base(nullptr) {}
  context(uint32_t maxbytes) : unique_base(JS_NewContext(maxbytes)), cx(unique_base::get()) {
    js::UseInternalJobQueues(cx);
    if (!JS::InitSelfHostedCode(cx)) {
      throw std::runtime_error("error initializing selfhosted code");
    }

    // create default globals
    // XXX: caller should be able to configure the global
    JSAutoRequest r(cx);
    JS::CompartmentOptions compartment_opts;
    globals =
        object(cx, JS_NewGlobalObject(cx, &detail::global_class, nullptr, JS::FireOnNewGlobalHook, compartment_opts));
    top_compartment = JS_EnterCompartment(cx, globals);
    if (!JS_InitStandardClasses(cx, globals)) {
      throw std::runtime_error("error initializing standard classes");
    }
  }

  ~context() {
    if (cx) {
      JSAutoRequest r(cx);
      JS_LeaveCompartment(cx, top_compartment);
    }
  }

  context(const context&) = delete;
  context& operator=(const context&) = delete;

  context(context&&) = default;
  context& operator=(context&& that) {
    cx = std::move(that.cx);
    that.cx = nullptr;
    globals = std::move(that.globals);
    top_compartment = std::move(that.top_compartment);
    unique_base::operator=(std::move(that));
    return *this;
  }

  operator JSContext*() const {
    return cx;
  }

  object global() {
    return globals;
  }

  value eval(const std::string& code, const std::string& filename = "noname", int lineno = 1) {
    JSAutoRequest r(cx);
    value rval(cx);
    JS::CompileOptions opts(cx);
    opts.setFileAndLine(filename.c_str(), lineno);
    JS::Evaluate(cx, opts, code.c_str(), code.length(), rval);
    return rval;
  }

 private:
  JSContext* cx = nullptr;
  JSCompartment* top_compartment = nullptr;
  object globals;
};

}  // namespace jspp
