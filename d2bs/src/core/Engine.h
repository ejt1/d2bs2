#pragma once

#include "scripting/ScriptEngine.h"
#include "utils/CommandLine.h"

#include <filesystem>

namespace d2bs {

class Engine {
 public:
  Engine(std::filesystem::path mod_path);
  ~Engine();

  bool Startup();
  void Shutdown();
  void WaitForShutdown();

  // called from game thread ONLY!
  void HandleUpdate();

 private:
  void ParseCmdLineArgs();

 private:
  // Path to our dll file
  std::filesystem::path mod_path_;
  // Path to the directory containing our module
  std::filesystem::path working_dir_;

  ScriptEngine script_engine_;
};

// An alternative to using singleton
extern Engine* s_engine;

}  // namespace d2bs
