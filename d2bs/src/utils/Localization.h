#pragma once

#include <Windows.h>
#include <algorithm>
#include <cinttypes>
#include <string>

namespace d2bs {
namespace util {

inline std::string WideToAnsi(const std::wstring& in, DWORD locale = CP_ACP) {
  std::string out;

  if (in.empty()) {
    return out;
  }

  // Calculate length of out string
  int32_t len = WideCharToMultiByte(locale, 0, in.c_str(), static_cast<int32_t>(in.length()), 0, 0, nullptr, nullptr);
  out.resize(len);

  // Convert
  WideCharToMultiByte(locale, 0, in.c_str(), static_cast<int32_t>(in.length()), out.data(), len, nullptr, nullptr);

  return out;
}

inline std::wstring AnsiToWide(const std::string& in, DWORD locale = CP_ACP) {
  std::wstring out;

  if (in.empty()) {
    return out;
  }

  // Calculate length of out string
  int32_t len = MultiByteToWideChar(locale, 0, in.c_str(), static_cast<int32_t>(in.length()), 0, 0);
  out.resize(len);

  // Convert
  MultiByteToWideChar(locale, 0, in.c_str(), static_cast<int32_t>(in.length()), out.data(), len);

  return out;
}

inline std::string WideToUTF8(const std::wstring& str) {
  return WideToAnsi(str, CP_UTF8);
}

inline std::wstring UTF8ToWide(const std::string& str) {
  return AnsiToWide(str, CP_UTF8);
}

inline std::string ToLower(const std::string& str) {
  std::string out = str;
  std::transform(out.begin(), out.end(), out.begin(),
                 [](unsigned char c) { return static_cast<unsigned char>(tolower(c)); });
  return out;
}

inline std::wstring ToLower(const std::wstring& str) {
  std::wstring out = str;
  std::transform(out.begin(), out.end(), out.begin(), [](wchar_t c) { return static_cast<wchar_t>(towlower(c)); });
  return out;
}
inline std::string ToUpper(const std::string& str) {
  std::string out = str;
  std::transform(out.begin(), out.end(), out.begin(),
                 [](unsigned char c) { return static_cast<unsigned char>(toupper(c)); });
  return out;
}

inline std::wstring ToUpper(const std::wstring& str) {
  std::wstring out = str;
  std::transform(out.begin(), out.end(), out.begin(), [](wchar_t c) { return static_cast<wchar_t>(towupper(c)); });
  return out;
}

}  // namespace util
}  // namespace d2bs
