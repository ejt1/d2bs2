#pragma once

#include "Profile.h"

#include <Windows.h>  // for HWND
#include <filesystem>
#include <map>
#include <vector>

namespace d2bs {

/**
 * @brief This whole thing is a shitshow because keeping compatibility with d2bs.ini. Would rather rewrite settings to
 * use yaml instead and yeeting most of the settings but want to keep compatibility with D2Bot for now, revisit this in
 * the future when D2Bot is on the chopping block :)
 */
class Settings {
  Settings();
  ~Settings();

 public:
  static Settings* GetInstance();

  bool LoadFile(const std::filesystem::path& filename);
  Profile LoadProfile(const std::string& filename, const std::string& section);

  void SetActiveProfile(const std::string& name);
  Profile ActiveProfile();

  void DebugPrint();

  // copy-pasted settings from d2bs, remove whatever is not necessary later
  // XXX: put these in a separate struct so we don't have a bunch of publicly exposed variables
  std::filesystem::path scriptDir;
  char scriptPath[_MAX_PATH]{};
  char szConsole[_MAX_PATH]{};
  char szDefault[_MAX_PATH]{};
  char szStarter[_MAX_PATH]{};

  char hosts[256]{};
  char debug[6]{};
  char quitOnHostile[6]{};
  char quitOnError[6]{};
  char startAtMenu[6]{};
  char disableCache[6]{};
  char gamePrint[6]{};
  char useProfilePath[6]{};
  char logConsole[6]{};
  char enableUnsupported[6]{};
  char forwardMessageBox[6]{};

  int32_t maxGameTime = 0;
  int32_t gameTimeout = 0;
  uint32_t dwGameTime = 0;
  uint32_t dwMaxGameTime = 0;
  uint32_t dwGameTimeout = 0;
  uint32_t dwMemUsage = 0;
  uint32_t dwConsoleFont = 0;
  uint32_t dwMaxLoginTime = 0;
  uint32_t dwMaxCharSelectTime = 0;

  bool bQuitOnHostile = false;
  bool bQuitOnError = false;
  bool bStartAtMenu = false;
  bool bDisableCache = false;
  bool bUseGamePrint = false;
  bool bUseProfileScript = false;
  bool bLogConsole = false;
  bool bEnableUnsupported = false;
  bool bForwardMessageBox = false;

  // settings not in d2bs.ini
  std::string szTitle;
  bool bSleepy = false;
  bool bCacheFix = false;
  bool bMulti = false;
  bool bReduceFTJ = false;
  bool bUseRawCDKey = false;
  std::string szClassic;
  std::string szLod;
  HWND hHandle = nullptr;

 private:
  std::vector<std::filesystem::path> loaded_;
  std::map<std::string, Profile> profiles_;
  std::string active_profile_;
};

#define sSettings ::d2bs::Settings::GetInstance()

}  // namespace d2bs
