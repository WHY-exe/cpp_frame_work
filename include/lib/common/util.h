#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace util {
static constexpr uint32_t MAX_BUFFER_SIZE = 1024;
static constexpr uint32_t MAX_PATH_LEN = 1024;
/**
 * @brief get the output string of executed command from cmdline
 * @param [in] cmd the specified command
 * @return the output string of the command
 */
namespace exec_cmd {
std::optional<std::string> GetLine(const std::string &cmd) noexcept;
std::optional<std::string> GetAll(const std::string &cmd) noexcept;
}; // namespace exec_cmd

std::vector<std::string> Split(const std::string &target,
                               const std::string &pattern);
uint64_t HexToDec(const std::string &hex);
std::string DecToHex(int dec);
bool ContainAlphaInStr(const std::string &target) noexcept;

std::string RemoveExtraSpaceInStr(const std::string &origin_str);
std::string RemoveExtraCharInStr(const std::string &origin_str,
                                 char c) noexcept;
std::string RemoveCharInStr(const std::string &target, char ch) noexcept;
std::string RemoveCharInStr(const std::string &target,
                            const std::string &chs) noexcept;
}; // namespace util