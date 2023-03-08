#include "core/Engine.h"
#include "core/Log.h"

#include <Windows.h>

// This is the only 'using namespace' allowed in this project :)
using namespace d2bs;

HANDLE g_hMainThread = nullptr;

DWORD __stdcall MainThread(LPVOID lpParam) {
  HMODULE hModule = static_cast<HMODULE>(lpParam);

  char modfile[MAX_PATH]{};
  GetModuleFileNameA(hModule, reinterpret_cast<char*>(&modfile), MAX_PATH);
  std::filesystem::path dll_path = modfile;
  std::filesystem::path root_dir = dll_path.parent_path();
  std::filesystem::path log_dir = root_dir / "logs";
  Log::Init(log_dir);

  try {
    d2bs::Engine engine(dll_path);
    if (engine.Startup()) {
      engine.WaitForShutdown();
      engine.Shutdown();
    } else {
      spdlog::error("Failed to wroom wroom :(");
    }
  } catch (const std::exception& e) {
    spdlog::critical("exception propagated to the top of the stack, this should NEVER happen.");
    spdlog::critical(
        "This is a fatal error and crash is inevitable, here's the error message we received, in case you care:");
    spdlog::critical("-- {}", e.what());
  }

  spdlog::debug("Shutting down main thread");
  FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  UNREFERENCED_PARAMETER(lpReserved);

  switch (dwReason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hModule);

      g_hMainThread = CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
      break;
    case DLL_PROCESS_DETACH:
      break;
  }

  return TRUE;
}
