#include "Log.h"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Windows.h>

namespace d2bs {

static FILE* s_outStream = nullptr;
static FILE* s_inStream = nullptr;

void Log::Init(const std::filesystem::path& dir) {
  std::vector<spdlog::sink_ptr> sinks;

#if defined(DEBUG)
  AllocConsole();
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleTitle("D2BS Console");
  freopen_s(&s_outStream, "CONOUT$", "w+t", stdout);
  freopen_s(&s_inStream, "CONIN$", "r+t", stdin);
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif
  sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((dir / "d2bs.txt").string(), 23, 55));

  auto logger = std::make_shared<spdlog::logger>("d2bs", sinks.begin(), sinks.end());

  spdlog::set_default_logger(logger);
#if defined(_DEBUG)
  spdlog::set_level(spdlog::level::trace);
#else
  spdlog::set_level(spdlog::level::info);
#endif
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%T.%e %l] %v");
}

void Log::Shutdown() {
#if defined(_DEBUG)
  fflush(stdout);
  fclose(s_outStream);
  fflush(stdin);
  fclose(s_inStream);
  FreeConsole();
#endif
}

}  // namespace d2bs
