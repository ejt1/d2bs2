#pragma once

#include "Log.h"

#include <string>

namespace d2bs {

enum class ProfileType { Invalid, SinglePlayer, BattleNet, OpenBattleNet, TcpIpHost, TcpIpJoin };

struct Profile {
  std::string name;

  ProfileType type = ProfileType::Invalid;
  std::string username;
  std::string password;
  std::string gateway;
  std::string charname;
  std::string diff;

  std::string scriptPath;
  std::string gameScript;
  std::string starterScript;

  void DebugPrint() const {
    spdlog::debug("-- Profile {}", name);
    switch (type) {
      case ProfileType::Invalid:
        spdlog::debug("type invalid");
        break;
      case ProfileType::SinglePlayer:
        spdlog::debug("type SinglePlayer");
        break;
      case ProfileType::BattleNet:
        spdlog::debug("type BattleNet");
        break;
      case ProfileType::OpenBattleNet:
        spdlog::debug("type OpenBattleNet");
        break;
      case ProfileType::TcpIpHost:
        spdlog::debug("type TcpIpHost");
        break;
      case ProfileType::TcpIpJoin:
        spdlog::debug("type TcpIpJoin");
        break;
    }
    spdlog::debug("username {}", username);
    spdlog::debug("password {}", password);
    spdlog::debug("gateway {}", gateway);
    spdlog::debug("charname {}", charname);
    spdlog::debug("diff {}", diff);
    spdlog::debug("scriptPath {}", scriptPath);
    spdlog::debug("gameScript {}", gameScript);
    spdlog::debug("starterScript {}", starterScript);
  }
};

}  // namespace d2bs
