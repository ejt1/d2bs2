#pragma once

#include <deque>
#include <sstream>

namespace d2bs {

// XXX: WIP do not use
class Console {
  Console() noexcept = default;
  ~Console() noexcept = default;

 public:
  static Console* GetInstance();

  void ToggleVisible();
  void SetVisible(bool visible);
  bool IsVisible();

  void ToggleEnabled();
  void SetEnabled(bool enabled);
  bool IsEnabled();

  void AddKey(char key);
  void RemoveLastKey();

  void ExecuteCommand();
  void PrevCommand();
  void NextCommand();

  void ScrollUp();
  void ScrollDown();

  void AddLine(std::string line);
  void Update();
  void Clear();
  void Draw();

 private:
  bool visible_ = false;
  bool enabled_ = false;

  std::deque<std::string> lines;
  std::deque<std::string> commands;
  std::deque<std::string> history;
  uint32_t lines_ = 14;
  uint32_t idx_ = 0;
  uint32_t width_ = 625;
  uint32_t height_ = 0;

  std::stringstream cmdbuffer_;
  uint32_t cmdpos_ = 0;
};

#define sConsole Console::GetInstance()

}  // namespace d2bs
