#pragma once

#include <map>
#include <string>
#include <vector>

namespace d2bs {
namespace util {

class CommandLine {
 public:
  CommandLine(std::string cmdline);
  ~CommandLine() noexcept = default;

  bool contains(const std::string& arg);
  std::string value(const std::string& arg);
  std::map<std::string, std::string> args();

 private:
  void trim_binary_path(std::string& cmdline);
  std::string& trim_quote(std::string& val);
  std::vector<std::string> parse_args(std::string& cmdline);

 private:
  std::map<std::string, std::string> args_;
};

}  // namespace util
}  // namespace d2bs
