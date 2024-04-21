#pragma once
#include "spdlog/spdlog.h"
namespace util {
namespace log {
std::string GetModuleName(std::string_view file_path);
std::string GetFileName(std::string_view file_path);
#ifdef WIN32
std::string FormatWin32Error(uint32_t error_num) noexcept;
#endif // WINDOWS
} // namespace log
} // namespace util

#define LOG_ERROR_COUT(fmt, ...)                                               \
  spdlog::error("[{}]({}:{}) " fmt, util::log::GetModuleName(__FILE__),        \
                util::log::GetFileName(__FILE__), __LINE__, ##__VA_ARGS__)

#define LOG_WARN_COUT(fmt, ...)                                                \
  spdlog::warn("[{}]({}:{}) " fmt, util::log::GetModuleName(__FILE__),         \
               util::log::GetFileName(__FILE__), __LINE__, ##__VA_ARGS__)

#ifdef WIN32
#define LOG_WIN_ERROR_COUT                                                     \
  LOG_ERROR_COUT("{}", util::log::FormatWin32Error(GetLastError()))
#endif
