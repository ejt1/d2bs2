#include "utils/CommandLine.h"

namespace d2bs {
namespace util {

CommandLine::CommandLine(std::string cmdline) {
  // example input C:\Program Files (x86)\Diablo II\Game.exe -profile "test" -handle "4325261" -cachefix -multi
  // -title "some title" -w -ns -sleepy -ftj
  trim_binary_path(cmdline);
  auto args = parse_args(cmdline);

  auto it = args.begin();
  while (it != args.end()) {
    std::string arg = (*it);
    std::string val;
    if (++it != args.end() && (*it)[0] != '-') {
      val = (*it);
      ++it;
    }
    args_[arg] = trim_quote(val);
  }
}

bool CommandLine::contains(const std::string& arg) {
  return args_.contains(arg);
}

std::string CommandLine::value(const std::string& arg) {
  if (!contains(arg)) {
    return "";
  }

  return args_[arg];
}

std::map<std::string, std::string> CommandLine::args() {
  return args_;
}

void CommandLine::trim_binary_path(std::string& cmdline) {
  size_t pos = 0;

  // remove the binary path from command line
  if ((pos = cmdline.find(".exe")) != std::string::npos) {
    cmdline.erase(0, pos + 4);

    // remove trailing space, if present
    if (!cmdline.empty() && cmdline[0] == ' ') {
      cmdline.erase(0, 1);
    }
  }
}

std::string& CommandLine::trim_quote(std::string& val) {
  if (val.starts_with('\"')) {
    val.erase(0, 1);
  }
  if (val.ends_with('\"')) {
    val.erase(val.size() - 1, val.size());
  }
  return val;
}

std::vector<std::string> CommandLine::parse_args(std::string& cmdline) {
  size_t pos = 0;
  std::vector<std::string> args;

  // tokenize the remainder of command line into arguments
  while ((pos = cmdline.find(' ')) != std::string::npos || !cmdline.empty()) {
    args.push_back(cmdline.substr(0, pos));

    // contains trailing space
    if (pos != std::string::npos) {
      cmdline.erase(0, pos + 1);
    } else {
      // last argument, clear the string to stop loop
      // ugly but best I can think of right now :)
      cmdline.clear();
    }
  }

  return args;
}

}  // namespace utils
}  // namespace d2bs
