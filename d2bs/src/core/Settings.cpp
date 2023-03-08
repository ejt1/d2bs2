#include "core/Settings.h"

#include "Log.h"

#include <Windows.h>

namespace d2bs {

// ascii -> bool
bool atob(const char* str) {
  if (str) {
    switch (tolower(str[0])) {
      case 't':
      case '1':
        return true;
      case 'f':
      case '0':
      default:
        return false;
    }
  }
  return false;
}

Settings::Settings() {}

Settings::~Settings() {}

Settings* Settings::GetInstance() {
  static Settings instance;
  return &instance;
}

bool Settings::LoadFile(const std::filesystem::path& filename) {
  if (!std::filesystem::exists(filename)) {
    spdlog::error("Settings file {} does not exist", filename.string());
    return false;
  }

  auto it = std::find_if(loaded_.begin(), loaded_.end(), [&](const std::filesystem::path& p) { return p == filename; });
  if (it != loaded_.end()) {
    spdlog::error("Settings file {} is already loaded", filename.string());
    return false;
  }

  // make a copy to keep it alive when we use .c_str()
  // ugly but will make due for now
  std::string cfilename = filename.string();
  const char* cfile = cfilename.c_str();

  char memUsage[6]{};
  char consoleFont[6]{};

  GetPrivateProfileString("settings", "ScriptPath", "scripts", scriptPath, _MAX_PATH, cfile);
  GetPrivateProfileString("settings", "DefaultConsoleScript", "", szConsole, _MAX_FNAME, cfile);
  GetPrivateProfileString("settings", "DefaultGameScript", "default.dbj", szDefault, _MAX_FNAME, cfile);
  GetPrivateProfileString("settings", "DefaultStarterScript", "starter.dbj", szStarter, _MAX_FNAME, cfile);
  GetPrivateProfileString("settings", "Hosts", "", hosts, 256, cfile);
  maxGameTime = GetPrivateProfileInt("settings", "MaxGameTime", 0, cfile);
  GetPrivateProfileString("settings", "Debug", "false", debug, 6, cfile);
  GetPrivateProfileString("settings", "QuitOnHostile", "false", quitOnHostile, 6, cfile);
  GetPrivateProfileString("settings", "QuitOnError", "false", quitOnError, 6, cfile);
  GetPrivateProfileString("settings", "StartAtMenu", "true", startAtMenu, 6, cfile);
  GetPrivateProfileString("settings", "DisableCache", "true", disableCache, 6, cfile);
  GetPrivateProfileString("settings", "MemoryLimit", "100", memUsage, 6, cfile);
  GetPrivateProfileString("settings", "UseGamePrint", "false", gamePrint, 6, cfile);
  gameTimeout = GetPrivateProfileInt("settings", "GameReadyTimeout", 5, cfile);
  GetPrivateProfileString("settings", "UseProfileScript", "false", useProfilePath, 6, cfile);
  GetPrivateProfileString("settings", "LogConsoleOutput", "false", logConsole, 6, cfile);
  GetPrivateProfileString("settings", "EnableUnsupported", "false", enableUnsupported, 6, cfile);
  GetPrivateProfileString("settings", "ForwardMessageBox", "false", forwardMessageBox, 6, cfile);
  GetPrivateProfileString("settings", "ConsoleFont", "0", consoleFont, 6, cfile);
  dwMaxLoginTime = GetPrivateProfileInt("settings", "MaxLoginTime", 5, cfile);
  dwMaxCharSelectTime = GetPrivateProfileInt("settings", "MaxCharSelectTime", 5, cfile);

  // script path relative to the config file
  scriptDir = filename.parent_path() / scriptPath;

  dwGameTime = GetTickCount();
  dwMaxGameTime = abs(maxGameTime * 1000);
  dwGameTimeout = abs(gameTimeout * 1000);

  bQuitOnHostile = atob(quitOnHostile);
  bQuitOnError = atob(quitOnError);
  bStartAtMenu = atob(startAtMenu);
  bDisableCache = atob(disableCache);
  bUseGamePrint = atob(gamePrint);
  bUseProfileScript = atob(useProfilePath);
  bLogConsole = atob(logConsole);
  bEnableUnsupported = atob(enableUnsupported);
  bForwardMessageBox = atob(forwardMessageBox);
  dwMemUsage = abs(atoi(memUsage));
  dwConsoleFont = abs(atoi(consoleFont));
  if (dwMemUsage < 1) {
    dwMemUsage = 500;
  }
  dwMemUsage *= 1024 * 1024;

  // profiles
  char profileNames[4096]{};
  uint32_t profileNamesLen = GetPrivateProfileSectionNamesA(profileNames, 4096, cfile);
  for (uint32_t i = 0, start = 0; i < profileNamesLen; ++i) {
    if (profileNames[i] == '\0') {
      std::string name(&profileNames[start], i - start);
      start = i + 1;

      if (!_stricmp(name.c_str(), "settings")) {
        continue; // skip settings section
      }

      profiles_[name] = LoadProfile(cfilename, name);
    }
  }

  loaded_.emplace_back(filename);
  return true;
}

Profile Settings::LoadProfile(const std::string& filename, const std::string& section) {
  char tmpMode[256]{}, tmpChar[256]{}, tmpDiff[256]{}, tmpUsername[256]{}, tmpPassword[256]{}, tmpGateway[256]{},
      tmpScriptPath[256]{}, tmpGameScript[256]{}, tmpStarterScript[256]{};

  GetPrivateProfileStringA(section.c_str(), "mode", "single", tmpMode, _countof(tmpMode), filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "character", "ERROR", tmpChar, _countof(tmpChar), filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "spdifficulty", "0", tmpDiff, _countof(tmpDiff), filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "username", "ERROR", tmpUsername, _countof(tmpUsername), filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "password", "ERROR", tmpPassword, _countof(tmpPassword), filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "gateway", "ERROR", tmpGateway, _countof(tmpGateway), filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "ScriptPath", "ERROR", tmpScriptPath, _countof(tmpScriptPath),
                           filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "DefaultGameScript", "ERROR", tmpGameScript, _countof(tmpGameScript),
                           filename.c_str());
  GetPrivateProfileStringA(section.c_str(), "DefaultStarterScript", "ERROR", tmpStarterScript, _countof(tmpStarterScript),
                           filename.c_str());

  ProfileType tmpType = ProfileType::Invalid;

  switch (tolower(tmpMode[0])) {
    case 's':
      tmpType = ProfileType::SinglePlayer;
      break;
    case 'b':
      tmpType = ProfileType::BattleNet;
      break;
    case 'o':
      tmpType = ProfileType::OpenBattleNet;
      break;
    case 'h':
      tmpType = ProfileType::TcpIpHost;
      break;
    case 'j':
      tmpType = ProfileType::TcpIpJoin;
      break;
  }

  Profile profile;
  profile.name = section;
  profile.type = tmpType;
  profile.username = tmpUsername;
  profile.password = tmpPassword;
  profile.gateway = tmpGateway;
  profile.charname = tmpChar;
  profile.diff = tmpDiff;
  profile.scriptPath = tmpScriptPath;
  profile.gameScript = tmpGameScript;
  profile.starterScript = tmpStarterScript;
  return profile;
}

void Settings::SetActiveProfile(const std::string& name) {
  if (!profiles_.contains(name)) {
    spdlog::error("Profile {} does not exist", name);
    return;
  }

  active_profile_ = name;
}

Profile Settings::ActiveProfile() {
  if (profiles_.contains(active_profile_)) {
    return profiles_[active_profile_];
  }

  return {};
}

void Settings::DebugPrint() {
  auto printSetting = [](const std::string& name, auto setting) { spdlog::debug("{} {}", name, setting); };

  spdlog::debug("-- Settings");
  printSetting("scriptDir", scriptDir.string());
  printSetting("scriptPath", scriptPath);
  printSetting("szConsole", szConsole);
  printSetting("szDefault", szDefault);
  printSetting("szStarter", szStarter);
  printSetting("hosts", hosts);
  printSetting("debug", debug);
  printSetting("quitOnHostile", quitOnHostile);
  printSetting("quitOnError", quitOnError);
  printSetting("startAtMenu", startAtMenu);
  printSetting("disableCache", disableCache);
  printSetting("gamePrint", gamePrint);
  printSetting("useProfilePath", useProfilePath);
  printSetting("logConsole", logConsole);
  printSetting("enableUnsupported", enableUnsupported);
  printSetting("forwardMessageBox", forwardMessageBox);
  printSetting("maxGameTime", maxGameTime);
  printSetting("gameTimeout", gameTimeout);
  printSetting("dwGameTime", dwGameTime);
  printSetting("dwMaxGameTime", dwMaxGameTime);
  printSetting("dwGameTimeout", dwGameTimeout);
  printSetting("dwMemUsage", dwMemUsage);
  printSetting("dwConsoleFont", dwConsoleFont);
  printSetting("dwMaxLoginTime", dwMaxLoginTime);
  printSetting("dwMaxCharSelectTime", dwMaxCharSelectTime);
  printSetting("bQuitOnHostile", bQuitOnHostile);
  printSetting("bQuitOnError", bQuitOnError);
  printSetting("bStartAtMenu", bStartAtMenu);
  printSetting("bDisableCache", bDisableCache);
  printSetting("bUseGamePrint", bUseGamePrint);
  printSetting("bUseProfileScript", bUseProfileScript);
  printSetting("bLogConsole", bLogConsole);
  printSetting("bEnableUnsupported", bEnableUnsupported);
  printSetting("bForwardMessageBox", bForwardMessageBox);
  printSetting("szTitle", szTitle);
  printSetting("bSleepy", bSleepy);
  printSetting("bCacheFix", bCacheFix);
  printSetting("bMulti", bMulti);
  printSetting("bReduceFTJ", bReduceFTJ);
  printSetting("bUseRawCDKey", bUseRawCDKey);
  printSetting("szClassic", szClassic);
  printSetting("szLod", szLod);
  printSetting("hHandle", reinterpret_cast<uintptr_t>(hHandle));

  for (const auto& [name, profile] : profiles_) {
    profile.DebugPrint();
  }
}

}  // namespace d2bs
