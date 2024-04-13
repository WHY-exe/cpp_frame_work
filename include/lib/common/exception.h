#pragma once
#include <cerrno>
#include <cstdint>
#include <exception>
#include <string>
#include <system_error>

#include "spdlog/spdlog.h"

namespace util {
namespace exception {
struct BasicInfo {
  uint32_t _line;
  std::string _file;
  std::string _func;
  std::string _msg;
  std::string _type;
  BasicInfo(uint32_t line, std::string &&file, std::string &&func,
            std::string &&msg, std::string &&type);
  ~BasicInfo() = default;
  BasicInfo(const BasicInfo &lv_rhs) = default;
  BasicInfo &operator=(const BasicInfo &lv_rhs) = default;
  BasicInfo(BasicInfo &&rv_rhs) noexcept = default;
  BasicInfo &operator=(BasicInfo &&rv_rhs) noexcept = default;
  std::string GenBasicInfo() const noexcept;
};

class Basic : public std::exception {
private:
  BasicInfo m_info;
  mutable std::string m_error_str;

public:
  Basic(uint32_t line, std::string &&file, std::string &&func,
        std::string &&msg, std::string &&type);
  Basic(const Basic &) = default;
  Basic &operator=(const Basic &) = default;
  Basic(Basic &&) = default;
  Basic &operator=(Basic &&) = default;
  ~Basic() override = default;
  const char *what() const noexcept override;
};
#ifdef WINDOWS
std::string FormatWin32Error(uint32_t error_num) noexcept;
#endif // WINDOWS

} // namespace exception
} // namespace util

#define GET_BASIC_INFO(error_type)                                             \
  util::exception::BasicInfo(__LINE__, __FILE__, __func__, "", error_type)     \
      .GenBasicInfo()

#define GET_BASIC_INFO_WITH_MSG(error_type, error_msg)                         \
  util::exception::BasicInfo(__LINE__, __FILE__, __func__, error_msg,          \
                             error_type)                                       \
      .GenBasicInfo()

#define THROW_EXCEPTION(error_msg, type)                                       \
  throw util::exception::Basic(__LINE__, __FILE__, __func__, error_msg, type)

#define LOG_ERROR_COUT(x)                                                      \
  spdlog::error("({}:{})[{}]: {}", __FILE__, __LINE__, __func__, x)

// #ifdef LINUX
#define THROW_SYSTEM_ERROR                                                     \
  throw std::system_error(std::error_code(errno, std::system_category()),      \
                          GET_BASIC_INFO("system error"))
#define LOG_SYSTEM_ERROR_COUT LOG_ERROR_COUT(strerror(errno))
// #endif

#ifdef WINDOWS
#define THROW_WIN_ERROR                                                        \
  throw std::system_error(                                                     \
      std::error_code(GetLastError(), std::system_category()),                 \
      GET_BASIC_INFO("win32 error"))

#define LOG_WIN_ERROR_COUT                                                     \
  LOG_ERROR_COUT(util::exception::FormatWin32Error(GetLastError()))

#endif // WINDOWS
