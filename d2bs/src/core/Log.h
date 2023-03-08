#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include <filesystem>

namespace d2bs {

class Log {
 public:
  static void Init(const std::filesystem::path& dir);
  static void Shutdown();
};

}  // namespace d2bs
