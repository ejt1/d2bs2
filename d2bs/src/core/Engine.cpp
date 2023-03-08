#include "Engine.h"

#include "Log.h"
#include "Settings.h"
#include "game/Game.h"
#include "utils/CommandLine.h"

#if DEBUG
#include "ExceptionHandler.h"
#endif

namespace d2bs {

Engine* s_engine = nullptr;

Engine::Engine(std::filesystem::path mod_path) : mod_path_(mod_path), working_dir_(mod_path.parent_path()) {
  s_engine = this;
#if DEBUG
  detail::RegisterExceptionHandler(working_dir_);
#endif
}

Engine::~Engine() {
  s_engine = nullptr;
}

bool Engine::Startup() {
  if (!sSettings->LoadFile(working_dir_ / "d2bs.ini")) {
    return false;
  }

  // After loading settings so that command line args overrides settings from d2bs.ini
  ParseCmdLineArgs();

  // shameless copy-paste from d2bs
  DefineOffsets();
  InstallPatches();
  InstallConditional();

  spdlog::info("D2BS v{} startup complete", D2BS_VERSION);
  return true;
}

void Engine::Shutdown() {
  RemovePatches();
  RemoveConditional();

  spdlog::info("D2BS v{} shutdown complete", D2BS_VERSION);
}

void Engine::WaitForShutdown() {
  // temporary loop to keep the engine alive, this will be replaced by a global atomic boolean that can be used to
  // shutdown the engine from anywhere in the framework
  while (true)
    ;
}

void Engine::HandleUpdate() {
  try {
    // Update first in case script engine is not initialized
    sGame->Update();

    static std::once_flag of;
    std::call_once(of, [&] { script_engine_.Init(); });

    script_engine_.Update();
  } catch (const std::exception& e) {
    // XXX: Should gracefully shutdown here so that accumulating exceptions doesn't crash the game.
    spdlog::critical(e.what());
  }
}

void Engine::ParseCmdLineArgs() {
  util::CommandLine cmdline(GetCommandLineA());
  for (const auto& [arg, val] : cmdline.args()) {
    if (arg == "-title") {
      sSettings->szTitle = val;
    } else if (arg == "-sleepy") {
      sSettings->bSleepy = true;
    } else if (arg == "-cachefix") {
      sSettings->bCacheFix = true;
    } else if (arg == "-multi") {
      sSettings->bMulti = true;
    } else if (arg == "-ftj") {
      sSettings->bReduceFTJ = true;
    } else if (arg == "-d2c") {
      sSettings->bUseRawCDKey = true;
      sSettings->szClassic = val;
    } else if (arg == "-d2x") {
      sSettings->szLod = val;
    } else if (arg == "-handle") {
      sSettings->hHandle = (HWND)atoi(val.c_str());
    } else if (arg == "-mpq") {
      // LoadMPQ(val.c_str());
    } else if (arg == "-profile") {
      sSettings->SetActiveProfile(val);
      spdlog::info("Switched to profile {}", val);
    }
  }
}

}  // namespace d2bs
