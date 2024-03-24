#pragma once
#include <string.h>

#include <cstdint>
#include <list>
#include <optional>
#include <string>
#include <vector>
namespace util {
constexpr int32_t MAX_BUFFER_SIZE = 1024;
constexpr uint32_t MAX_PATH_LEN = 1024;
namespace exec_cmd {
std::string GetLine(const std::string& cmd);
std::string GetAll(const std::string& cmd);
};  // namespace exec_cmd

std::vector<std::string> Split(const std::string& target,
                               const std::string& pattern);
uint64_t HexToDec(const std::string& hex);
std::string DecToHex(int dec);
std::string ParsePath(const std::string& path);
bool ContainAlphaInStr(const std::string& target) noexcept;

std::string RemoveExtraSpaceInStr(const std::string& origin_str) noexcept;
std::string RemoveExtraCharInStr(const std::string& origin_str,
                                 char c) noexcept;
std::string RemoveCharInStr(const std::string& target, char ch) noexcept;
std::string RemoveCharInStr(const std::string& target,
                            const std::string& chs) noexcept;
void test();
};  // namespace util