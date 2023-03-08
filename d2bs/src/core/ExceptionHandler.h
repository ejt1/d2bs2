#pragma once

// clang-format off
#include <Windows.h>
#include <DbgHelp.h>
// clang-format on

#include <filesystem>

namespace d2bs {
namespace detail {

void RegisterExceptionHandler(const std::filesystem::path& dump_dir);
void CreateMinidump(EXCEPTION_POINTERS* ep);
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* ep);

}  // namespace detail
}  // namespace d2bs
