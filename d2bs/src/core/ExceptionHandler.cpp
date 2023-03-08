#include "ExceptionHandler.h"

#include "Settings.h"  // for working directory

namespace d2bs {
namespace detail {

static std::filesystem::path s_dump_dir;

void RegisterExceptionHandler(const std::filesystem::path& dump_dir) {
  s_dump_dir = dump_dir;
  SetUnhandledExceptionFilter(ExceptionHandler);
}

void CreateMinidump(EXCEPTION_POINTERS* ep) {
  if (!std::filesystem::exists(s_dump_dir)) {
    spdlog::trace("creating dump directories {}", s_dump_dir.string());
    std::filesystem::create_directories(s_dump_dir);
  }

  if (!std::filesystem::is_directory(s_dump_dir)) {
    spdlog::critical("could not write dump because {} is not a directory", s_dump_dir.string());
    return;
  }

  std::string filename = (s_dump_dir / "crash.dmp").string();
  HANDLE file =
      CreateFileA(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (file == INVALID_HANDLE_VALUE) {
    spdlog::critical("failed to create dump file");
    return;
  }

  MINIDUMP_EXCEPTION_INFORMATION ex_mini{};
  ex_mini.ThreadId = GetCurrentThreadId();
  ex_mini.ExceptionPointers = ep;
  ex_mini.ClientPointers = false;

  MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file,
                    MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory), ep ? &ex_mini : nullptr, nullptr, nullptr);

  CloseHandle(file);
}

LONG __stdcall ExceptionHandler(EXCEPTION_POINTERS* ep) {
  CreateMinidump(ep);

  // XXX: Generate a useful exception here :)
  // Exception needs to be caught by the same thread causing the exception
  throw std::runtime_error("exception");
}

}  // namespace detail
}  // namespace d2bs
