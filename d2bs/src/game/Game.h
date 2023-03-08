#pragma once

#include "Offset.h"
#include "Ptrs.h"

#include <format>
#include <string>

namespace d2bs {

enum class GameState { Null, Menu, InGame, Busy };

class Game {
  Game() noexcept = default;
  ~Game() noexcept = default;

 public:
  static Game* GetInstance();

  // Called from game hooks
  void Update();

  template <typename... Args>
  void Say(const std::string_view& fmt, Args&&... args) {
    SayFormatted(std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
  }

  void SayFormatted(const std::string& str);

  GameState& State();

 private:
  GameState state_;
};

#define sGame Game::GetInstance()

}  // namespace d2bs
