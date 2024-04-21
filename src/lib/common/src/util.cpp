#include "util.h"
#include "spdlog/fmt/bundled/core.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace util {
std::optional<std::string> exec_cmd::GetLine(const std::string &cmd) noexcept {
  std::unique_ptr<FILE, std::function<void(FILE *)>> file(
      popen(cmd.c_str(), "r"), [](FILE *pProc) -> void {
        if (pProc) {
          pclose(pProc);
          pProc = nullptr;
        }
      });

  char result_buf[MAX_BUFFER_SIZE] = {0};
  if (file == nullptr) {
    return std::nullopt;
  }
  while (fgets(result_buf, MAX_BUFFER_SIZE, file.get())) {
    if (strlen(result_buf)) {
      char last_char = result_buf[strlen(result_buf) - 1];
      result_buf[strlen(result_buf) - 1] = last_char == '\n' ? '\0' : last_char;
      break;
    }
  }
  return result_buf;
}

std::optional<std::string> exec_cmd::GetAll(const std::string &cmd) noexcept {
  std::unique_ptr<FILE, std::function<void(FILE *)>> file(
      popen(cmd.c_str(), "r"), [](FILE *pProc) -> void {
        if (pProc) {
          pclose(pProc);
          pProc = nullptr;
        }
      });
  char result_buf[MAX_BUFFER_SIZE] = {0};
  std::string result;
  if (file == nullptr) {
    return std::nullopt;
  }
  while (fgets(result_buf, MAX_BUFFER_SIZE, file.get())) {
    if (strlen(result_buf)) {
      result += result_buf;
    }
  }
  return result;
}

std::vector<std::string> Split(const std::string &target,
                               const std::string &pattern) {
  size_t last_start_pos = 0;
  std::vector<std::string> result{};
  for (size_t cur_pos = target.find(pattern, last_start_pos);
       cur_pos != std::string::npos;
       cur_pos = target.find(pattern, last_start_pos)) {
    result.emplace_back(
        target.substr(last_start_pos, cur_pos - last_start_pos));
    last_start_pos = cur_pos + pattern.length();
  }
  result.emplace_back(
      target.substr(last_start_pos, target.length() - last_start_pos));
  return result;
}

std::string RemoveExtraSpaceInStr(const std::string &origin_str) {
  bool alert = false;
  std::string result = "";
  for (auto i : origin_str) {
    if (i == '\t') {
      i = ' ';
    }
    if (!alert && i == ' ') {
      result += i;
      alert = true;
    }
    if (alert && i != ' ') {
      alert = false;
    }
    if (alert) {
      continue;
    }
    result += i;
  }
  return result;
}

std::string RemoveExtraCharInStr(const std::string &origin_str,
                                 char c) noexcept {
  bool alert = false;
  std::string result{};
  std::copy_if(origin_str.begin(), origin_str.end(), result.begin(),
               [c, &alert](char target_cur) -> bool {
                 if (!alert && target_cur == c) {
                   alert = true;
                 }
                 if (alert && target_cur != c) {
                   alert = false;
                 }
                 return !alert;
               });
  return result;
}

bool ContainAlphaInStr(const std::string &target) noexcept {
  const auto it =
      std::find_if(target.begin(), target.end(),
                   [](char cur) -> bool { return isalpha(cur) != 0; });
  return it != target.end();
}

uint64_t HexToDec(const std::string &hex) {
  std::unordered_map<char, size_t> hex_map{};
  char cHexInit = 'A';
  char dec_int = '0';
  const size_t hex_int = 10;
  for (size_t i = 0; i < hex_int; i++) {
    hex_map.insert(std::make_pair(dec_int++, i));
  }
  for (size_t i = hex_int; i < 16; i++) {
    hex_map.insert(std::make_pair(cHexInit++, i));
  }
  uint64_t result = 0;
  size_t x = hex.length() - 1;
  for (auto i : hex) {
    result += hex_map[i] * (uint64_t)pow(16.0, (double)x);
    x--;
  }
  return result;
}

std::string DecToHex(int dec) { return fmt::format("{:#X}", dec); }

std::string RemoveCharInStr(const std::string &target, char ch) noexcept {
  std::string strRet{};
  std::copy_if(target.begin(), target.end(), strRet.begin(),
               [ch](char target_cur) -> bool { return target_cur != ch; });
  return strRet;
}

std::string RemoveCharInStr(const std::string &target,
                            const std::string &chs) noexcept {
  std::string strRet{};
  std::copy_if(target.begin(), target.end(), strRet.begin(),
               [&chs](char target_cur) -> bool {
                 bool bPass = false;
                 for (const auto i : chs) {
                   if (bPass) {
                     break;
                   }
                   bPass = bPass || (target_cur == i);
                 }
                 return !bPass;
               });
  return strRet;
}
} // namespace util