#include "Console.h"

#include "Log.h"
#include "Settings.h"

namespace d2bs {

Console* d2bs::Console::GetInstance() {
  static Console instance;
  return &instance;
}

void Console::ToggleVisible() {
  visible_ = !visible_;
}

void Console::SetVisible(bool visible) {
  visible_ = visible;
}

bool Console::IsVisible() {
  return visible_;
}

void Console::ToggleEnabled() {
  enabled_ = !enabled_;
}

void Console::SetEnabled(bool enabled) {
  enabled_ = enabled;
}

bool Console::IsEnabled() {
  return enabled_;
}

void Console::AddKey(char key) {
  cmdbuffer_ << key;
}

void Console::RemoveLastKey() {
  if (cmdbuffer_.str().empty()) {
    return;
  }

  std::string cmd(cmdbuffer_.str());
  size_t len = cmd.length() - 1;
  cmdbuffer_.str(cmd.substr(0, len - 1));
  if (len > 0) {
    cmdbuffer_.seekg(len);
    cmdbuffer_.seekp(len);
  }
}

void Console::ExecuteCommand() {
  if (cmdbuffer_.str().empty()) {
    return;
  }

  commands.push_back(cmdbuffer_.str());
  cmdpos_ = commands.size();
  spdlog::debug("!NYI Process command {}", cmdbuffer_.str());
  cmdbuffer_.clear();
}

void Console::PrevCommand() {
  if (cmdpos_ < 1 || cmdpos_ > commands.size()) {
    cmdbuffer_.clear();
  } else {
    if (cmdpos_ >= 1) {
      --cmdpos_;
    }
    cmdbuffer_.str(commands[cmdpos_]);
  }
}

void Console::NextCommand() {
  if (cmdpos_ >= commands.size()) {
    return;
  }

  cmdbuffer_.str(commands[cmdpos_]);
  if (cmdpos_ < commands.size() - 1) {
    ++cmdpos_;
  }
}

void Console::ScrollUp() {
  if (idx_ == 0 || history.size() - idx_ == 0) {
    return;
  }

  --idx_;
  Update();
}

void Console::ScrollDown() {
  if (history.size() < lines_ || (history.size() - lines_ == idx_)) {
    return;
  }

  ++idx_;
  Update();
}

void Console::AddLine(std::string line) {
  history.push_back(line);
  while (history.size() > 300) {
    history.pop_front();
  }

  if (sSettings->bLogConsole) {
    spdlog::info(line.c_str());
  }

  idx_ = history.size() < lines_ ? 0 : history.size() - lines_;
  Update();
}

void Console::Update() {
  lines.clear();
  for (size_t i = history.size() - idx_; i > 0 && lines.size() < lines_; --i) {
    lines.push_back(history.at(history.size() - i));
  }
}

void Console::Clear() {
  lines.clear();
}

void Console::Draw() {
  if (IsVisible()) {
    // XXX: fix me
  }
}

}  // namespace d2bs
