#pragma once
#include "spdlog/common.h"
#include "spdlog/details/log_msg.h"
#include "spdlog/pattern_formatter.h"
#include <memory>

namespace util {
namespace log {
class ModuleFlagFormatter : public spdlog::custom_flag_formatter {
public:
  void format(const spdlog::details::log_msg &log_msg, const std::tm &,
              spdlog::memory_buf_t &dest) override;
  std::unique_ptr<custom_flag_formatter> clone() const override;
};
std::string GetModuleName(std::string_view file_path);
std::string GetFileName(std::string_view file_path);
#ifdef WIN32
std::string FormatWin32Error(uint32_t error_num) noexcept;
#endif // WINDOWS
} // namespace log
} // namespace util
