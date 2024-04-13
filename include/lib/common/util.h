#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace util {
static constexpr uint32_t MAX_BUFFER_SIZE = 1024;
static constexpr uint32_t MAX_PATH_LEN = 1024;
/**
 * @brief get the output string of executed command from cmdline
 * @param [in] cmd the specified command
 * @return the output string of the command
 * @note these two function would throw system_error if popen is failed
 */
namespace exec_cmd {
std::string GetLine(const std::string &cmd);
std::string GetAll(const std::string &cmd);
}; // namespace exec_cmd

std::vector<std::string> Split(const std::string &target,
                               const std::string &pattern);
uint64_t HexToDec(const std::string &hex);
std::string DecToHex(int dec);
std::string ParsePath(const std::string &path);
bool ContainAlphaInStr(const std::string &target) noexcept;

std::string RemoveExtraSpaceInStr(const std::string &origin_str) noexcept;
std::string RemoveExtraCharInStr(const std::string &origin_str,
                                 char c) noexcept;
std::string RemoveCharInStr(const std::string &target, char ch) noexcept;
std::string RemoveCharInStr(const std::string &target,
                            const std::string &chs) noexcept;
void test();
}; // namespace util