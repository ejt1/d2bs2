#pragma once

#include <jspp/jspp.h>

#include <assert.h>

namespace d2bs {

struct ScriptContext {
  static ScriptContext* GetFromJS(JSContext* cx) {
    ScriptContext* sc = static_cast<ScriptContext*>(JS_GetContextPrivate(cx));
    assert(sc);
    return sc;
  }
};

/**
 * @brief Important, read me!
 * There's a big restriction on the script engine because of the fact that the JS context
 * is created on the game thread, therefore it is very important that all calls using
 * this context is within the game thread otherwise bad things will happen :)
 */
class ScriptEngine {
 public:
  ScriptEngine();
  ~ScriptEngine();

  bool Init();
  void Shutdown();

  // Called inside the game thread, do everything we need to do
  void Update();

 private:
  bool init_ = false;

  // Makes sure SpiderMonkey is initialized and shutdown properly
  jspp::init_guard jsinit_;

  // Created inside the game thread! Anything using this context must be inside the game thread!
  jspp::context cx_;
};

}  // namespace d2bs
